use std::{any::{type_name, TypeId}, collections::VecDeque, env, ffi::OsStr, fmt, fs, panic::catch_unwind, path::Path, str::FromStr};

use colored::Colorize;
use num_traits::Num;
use proc_macro::TokenStream;
use syn::{parse_macro_input, token::{Mod, Use}, LitStr};

pub fn include_bluebrick_bindings(item: TokenStream) -> TokenStream {
    unsafe { env::set_var("COLORTERM", "truecolor") };
    let root = parse_macro_input!(item as LitStr);

    let items = parse_bb_bindings(&root.value());
    println!("Found {} files", items.len());
    for item in items {
        match item {
            Ok(item) => println!("{}", item.name()),
            Err(e) => println!("{e}")
        }
    }

    TokenStream::new()
}

#[derive(Debug)]
enum ParseError {
    UnexpectedWord { word: String, options: Vec<String> },
    UnexpectedChar { char: char, options: Vec<char> },
    InvalidChar { word: String, options: Vec<char> },
    InvalidNumber { word: String, num_type: String, e: Box<dyn std::error::Error> },
    FoundEOFWord { options: Vec<String> },
    FoundEOFChar { options: Vec<char> },
    FoundEOFNum { num_type: String },
}

impl fmt::Display for ParseError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let string = match self {
            ParseError::UnexpectedWord{ word, options } => format!("Unexpected word \"{word}\", expected one of {options:?}"),
            ParseError::UnexpectedChar { char, options } => format!("Unexpected symbol '{char}', expected one of {options:?}"),
            ParseError::InvalidChar { word, options } => format!("Unexpected word \"{word}\", expected one of {options:?}"),
            ParseError::InvalidNumber{ word, num_type, e } => format!("\"{word}\" is not a valid {num_type}: {e}"),
            ParseError::FoundEOFWord { options } => format!("Found EOF, expected one of {options:?}"),
            ParseError::FoundEOFChar { options } => format!("Found EOF, expected one of {options:?}"),
            ParseError::FoundEOFNum { num_type } => format!("Found EOF, expected {num_type}"),
        };
        write!(f, "{string}")
    }
}

// if you are reading this and need better error messages, make an issue, I'll see if I want to add onto this
#[derive(Debug)]
struct Error {
    file_name: String,
    char_rect: CharRect,
    err: ParseError,
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Failed to parse {}.bb {}: {}", self.file_name, self.char_rect, self.err)
    }
}

type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, Copy, Default)]
struct CharRect {
    start_line: u32,
    start_column: u32,
    end_line: u32,
    end_column: u32,
}

impl fmt::Display for CharRect {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "from {}, {}, to {}, {}", self.start_line, self.start_column, self.end_line, self.end_column)
    }
}

impl From<((u32, u32), (u32, u32))> for CharRect {
    fn from(value: ((u32, u32), (u32, u32))) -> Self {
        CharRect { start_line: value.0.0, start_column: value.0.1, end_line: value.1.0, end_column: value.1.1 }
    }
}

#[derive(Clone)]
struct Parser {
    file_name: String,
    chars: VecDeque<char>,
    current_line: u32,
    current_column: u32,
    word_line: u32,
    word_column: u32,
    token_line: u32,
    token_column: u32,
}

impl Parser {
    fn new(file_name: &str, chars: &str) -> Self {
        let chars = chars.split(|c| c == '\n' || c == '\r').filter_map(|line| {
            let line = line.split_whitespace().collect::<Vec<_>>().join(" ");
            match line.len() {
                0 => None,
                _ => Some(line),
            }
        }).collect::<Vec<_>>().join("\n");

        Self {
            file_name: String::from(file_name),
            chars: chars.chars().collect(),
            word_line: 0,
            word_column: 0,
            current_line: 0,
            current_column: 0,
            token_line: 0,
            token_column: 0,
        }
    }

    const RESERVED: [char; 11] = [ ':', '=', '[', '{', ']', '}', ',', '(', ')', '<', '>' ];

    fn pop_char(&mut self, first: bool) -> Option<char> {
        let char = self.chars.front().copied()?;

        if Self::RESERVED.contains(&char) && !first {
            None
        } else {
            if char == '\n' {
                self.current_line += 1;
                self.current_column = 0;
            } else {
                self.current_column += 1;
            }

            self.chars.pop_front();
            if char.is_whitespace() {
                None
            } else {
                Some(char)
            }
        }
    }

    pub fn peek_chars(&self, len: usize) -> String {
        self.chars.iter().take(len).collect()
    }

    fn drop_chars(&mut self, len: usize) {
        for _ in 0..len {
            self.chars.pop_front();
        }
    }

    pub fn has_chars(&self) -> bool {
        self.chars.len() > 0
    }

    fn save_word_pos(&mut self) {
        self.word_line = self.current_line;
        self.word_column = self.current_column;
    }

    fn make_error(&self, err: ParseError) -> Error {
        Error {
            file_name: self.file_name.clone(),
            char_rect: CharRect {
                start_line: self.word_line,
                start_column: self.word_column,
                end_line: self.current_line,
                end_column: self.current_column,
            },
            err
        }
    }

    pub fn current_pos(&self) -> (u32, u32) {
        (self.current_line, self.current_column)
    }

    pub fn word(&mut self) -> String {
        self.save_word_pos();
        let mut result = String::new();

        let char = self.pop_char(true);
        if let Some(char) = char {
            result.push(char);

            if Self::RESERVED.contains(&char) {
                if self.chars.front().is_some_and(|char| char.is_whitespace()) {
                    self.chars.pop_front();
                }
                return result;
            }
        }

        while let Some(char) = self.pop_char(false) {
            result.push(char);
        }

        result
    }

    pub fn expect_word(&mut self, options: &[&str]) -> Result<String> {
        let collect_options = || options.iter().map(|o| String::from(*o)).collect();

        let word = self.word();
        if word.chars().count() == 0 {
            Err(self.make_error(ParseError::FoundEOFWord { options: collect_options() }))
        } else if options.contains(&word.as_str()) {
            Ok(word)
        } else {
            Err(self.make_error(ParseError::UnexpectedWord {
                word: String::from(word),
                options: collect_options()
            }))
        }
    }

    pub fn expect_char(&mut self, options: &[char]) -> Result<char> {
        let collect_options = || options.iter().copied().collect();

        let word = self.word();
        match word.chars().count() {
            0 => return Err(self.make_error(ParseError::FoundEOFChar { options: collect_options() })),
            1 => {}
            _ => return Err(self.make_error(ParseError::InvalidChar {
                word,
                options: collect_options(),
            }))
        }
        let char = word.chars().nth(0).unwrap();

        if options.contains(&char) {
            Ok(char)
        } else {
            Err(self.make_error(ParseError::UnexpectedChar {
                char,
                options: collect_options()
            }))
        }
    }

    fn parse_multibase<T: Num + FromStr>(&self, num: &str) -> Result<T>
    where
        T::FromStrRadixErr: std::error::Error + 'static,
        T::Err: std::error::Error + 'static,
    {
        let create_error = |e| {
            self.make_error(ParseError::InvalidNumber {
                word: String::from(num),
                num_type: String::from(type_name::<T>()),
                e,
            })
        };

        let prefix = if num.len() >= 2 { &num[..2] } else { "" };
        let radix = match prefix {
            "0b" => 2,
            "0o" => 8,
            "0x" => 16,
            _ => return match num.parse::<T>() {
                Ok(num) => Ok(num),
                Err(e) => Err(create_error(Box::new(e)))
            }
        };
        match T::from_str_radix(num.strip_prefix(prefix).unwrap(), radix) {
            Ok(num) => Ok(num),
            Err(e) => Err(create_error(Box::new(e)))
        }
    }

    pub fn expect_num<T: Num + FromStr>(&mut self) -> Result<T>
    where
        T::FromStrRadixErr: std::error::Error + 'static,
        T::Err: std::error::Error + 'static,
    {
        let num = self.word();
        if num.chars().count() == 0 {
            Err(self.make_error(ParseError::FoundEOFNum { num_type: String::from(type_name::<T>()) }))
        } else {
            Ok(self.parse_multibase(&num)?)
        }
    }
}

#[derive(Debug)]
struct File {
    name: String,
    contents: String,
}

trait Token {
    fn parse(parser: &mut Parser) -> Result<Self> where Self: Sized;
    fn char_rect(&self) -> CharRect;
    fn to_rust(&self) -> TokenStream;
}

enum ConcreteToken {
    Class(ClassToken),
    Module(ModuleToken),
    Fields(FieldsToken),
    Implements(ImplementsToken),
    Overrides(OverridesToken),
    Virtuals(VirtualsToken),
}

trait FileToken : Token {
    fn name(&self) -> &String;
    fn module(&self) -> &ModuleToken;
    fn used_modules(&self) -> &Vec<ModuleToken>;
    fn children(&self) -> &Vec<ConcreteToken>;
}

enum ConcreteFileToken {
    ClassToken(ClassToken),
}

impl ConcreteFileToken {
    fn get_inner(&self) -> &impl FileToken {
        match self {
            ConcreteFileToken::ClassToken(class) => class
        }
    }
}

impl Token for ConcreteFileToken {
    fn parse(_: &mut Parser) -> Result<Self> where Self: Sized {
        unimplemented!("Don't try to parse from an enum")
    }

    fn char_rect(&self) -> CharRect {
        self.get_inner().char_rect()
    }

    fn to_rust(&self) -> TokenStream {
        self.get_inner().to_rust()
    }
}

impl FileToken for ConcreteFileToken {
    fn name(&self) -> &String {
        self.get_inner().name()
    }

    fn module(&self) -> &ModuleToken {
        self.get_inner().module()
    }

    fn used_modules(&self) -> &Vec<ModuleToken> {
        self.get_inner().used_modules()
    }

    fn children(&self) -> &Vec<ConcreteToken> {
        self.get_inner().children()
    }
}

struct ModuleToken {
    name: String,
    parents: Vec<String>,
    char_rect: CharRect,
}

impl Token for ModuleToken {
    fn parse(parser: &mut Parser) -> Result<Self> {
        println!("Module");

        let start = parser.current_pos();

        let name = parser.word();
        let mut parents = Vec::new();
        while parser.peek_chars(2) == "::" {
            parser.drop_chars(2);
            parents.push(parser.word());
        }

        let end = parser.current_pos();
        Ok(ModuleToken {
            name,
            parents,
            char_rect: CharRect::from((start, end)),
        })
    }
    
    fn char_rect(&self) -> CharRect {
        self.char_rect
    }
    
    fn to_rust(&self) -> TokenStream {
        todo!("Implement ModuleToken to_rust")
    }
}

struct ClassToken {
    name: String,
    module: ModuleToken,
    used_modules: Vec<ModuleToken>,
    children: Vec<ConcreteToken>,
    char_rect: CharRect,
}

impl Token for ClassToken {
    fn parse(parser: &mut Parser) -> Result<Self> {
        println!("Class");

        let start = parser.current_pos();
        parser.expect_word(&["mod"])?;
        let module = ModuleToken::parse(parser)?;
        let mut used_modules = Vec::new();
        let mut children = Vec::new();

        while parser.has_chars() {
            match parser.expect_word(&["use", "fields", "implements", "overrides", "virtuals"])?.as_str() {
                "use" => used_modules.push(ModuleToken::parse(parser)?.into()),
                "fields" => children.push(ConcreteToken::Fields(FieldsToken::parse(parser)?)),
                "implements" => children.push(ConcreteToken::Implements(ImplementsToken::parse(parser)?)),
                "overrides" => children.push(ConcreteToken::Overrides(OverridesToken::parse(parser)?)),
                "virtuals" => children.push(ConcreteToken::Virtuals(VirtualsToken::parse(parser)?)),
                child_type => todo!("Implement class child type: {}", child_type),
            }
        }

        let end = parser.current_pos();
        Ok(ClassToken {
            name: parser.file_name.clone(),
            module,
            used_modules,
            children,
            char_rect: CharRect::from((start, end))
        })
    }
    
    fn char_rect(&self) -> CharRect {
        self.char_rect
    }

    fn to_rust(&self) -> TokenStream {
        todo!("Implement ClassToken to_rust")
    }
}

impl FileToken for ClassToken {
    fn name(&self) -> &String {
        &self.name
    }

    fn module(&self) -> &ModuleToken {
        &self.module
    }

    fn used_modules(&self) -> &Vec<ModuleToken> {
        &self.used_modules
    }

    fn children(&self) -> &Vec<ConcreteToken> {
        &self.children
    }
}

struct FieldsToken {
    fields: ListToken<FieldToken, '[', ']'>,
    char_rect: CharRect,
}

impl Token for FieldsToken {
    fn parse(parser: &mut Parser) -> Result<Self> where Self: Sized {
        println!("Fields");

        let start = parser.current_pos();
        let end = parser.current_pos();
        parser.expect_char(&['='])?;
        Ok(FieldsToken {
            fields: ListToken::parse(parser)?,
            char_rect: CharRect::from((start, end)),
        })
    }
    
    fn char_rect(&self) -> CharRect {
        self.char_rect
    }

    fn to_rust(&self) -> TokenStream {
        todo!("Implement FieldsToken to_rust")
    }
}

struct ListToken<T: Token, const open: char, const close: char> {
    items: Vec<T>,
    char_rect: CharRect,
}

impl<T: Token, const open: char, const close: char> Token for ListToken<T, open, close> {
    fn parse(parser: &mut Parser) -> Result<Self> where Self: Sized {
        println!("List");

        let start = parser.current_pos();

        let mut items = Vec::new();
        parser.expect_char(&[open])?;
        loop {
            let mut next = parser.peek_chars(1);
            if next == "," {
                parser.word();
                next = parser.peek_chars(1);
            }
            if next == close.to_string() {
                parser.word();
                break;
            }
            items.push(T::parse(parser)?);
        }

        let end = parser.current_pos();
        Ok(ListToken {
            items,
            char_rect: CharRect::from((start, end)),
        })
    }

    fn char_rect(&self) -> CharRect {
        self.char_rect
    }

    fn to_rust(&self) -> TokenStream {
        todo!("ListToken to_rust")
    }
}

struct FieldToken {
    offset: u32,
    variable: VariableToken,
    char_rect: CharRect,
}

impl Token for FieldToken {
    fn parse(parser: &mut Parser) -> Result<Self> where Self: Sized {
        println!("Field");

        let start = parser.current_pos();

        let offset = parser.expect_num::<u32>()?;
        parser.expect_char(&['='])?;
        let variable = VariableToken::parse(parser)?;

        print!("{} = {}: ", offset, variable.name);
        variable.r#type.print();
        println!("");
        let end = parser.current_pos();
        Ok(Self {
            offset,
            variable,
            char_rect: CharRect::from((start, end)),
        })
    }

    fn char_rect(&self) -> CharRect {
        self.char_rect
    }

    fn to_rust(&self) -> TokenStream {
        todo!("FieldToken to_rust")
    }
}

struct VariableToken {
    name: String,
    r#type: TypeToken,
    char_rect: CharRect,
}

impl Token for VariableToken {
    fn parse(parser: &mut Parser) -> Result<Self> where Self: Sized {
        println!("Variable");

        let start = parser.current_pos();

        let name = parser.word();
        parser.expect_char(&[':'])?;
        let r#type = TypeToken::parse(parser)?;

        let end = parser.current_pos();
        Ok(Self {
            name,
            r#type,
            char_rect: CharRect::from((start, end)),
        })
    }

    fn char_rect(&self) -> CharRect {
        self.char_rect
    }

    fn to_rust(&self) -> TokenStream {
        todo!("VariableToken to_rust")
    }
}

#[derive(Default)]
struct TypeToken {
    name: String,
    is_array: bool,
    array_len: u32,
    is_pointer: bool,
    // TODO: implement generics
    is_generic: bool,
    sub_types: Vec<TypeToken>,
    char_rect: CharRect,
}

impl TypeToken {
    fn print(&self) {
        if self.is_array {
            print!("array {}: ", self.array_len);
            self.sub_types[0].print();
        } else if self.is_pointer {
            print!("ptr ");
            self.sub_types[0].print();
        } else {
            print!("{}", self.name);
        }
    }
}

impl Token for TypeToken {
    fn parse(parser: &mut Parser) -> Result<Self> where Self: Sized {
        println!("Type");

        let start = parser.current_pos();

        let mut new = Self::default();
        let word = parser.word();
        match word.as_str() {
            "arr" => {
                let len = parser.expect_num::<u32>()?;
                new.is_array = true;
                new.array_len = len;
                new.sub_types.push(TypeToken::parse(parser)?);
            }
            "ptr" => {
                new.is_pointer = true;
                new.sub_types.push(TypeToken::parse(parser)?);
            }
            name => new.name = String::from(name),
        }

        let end = parser.current_pos();
        new.char_rect = CharRect::from((start, end));
        Ok(new)
    }

    fn char_rect(&self) -> CharRect {
        self.char_rect
    }

    fn to_rust(&self) -> TokenStream {
        todo!("TypeToken to_rust")
    }
}

struct ImplementsToken {

}

impl Token for ImplementsToken {
    fn parse(parser: &mut Parser) -> Result<Self> where Self: Sized {
        todo!("Implement ImplementsToken parse")
    }
    
    fn char_rect(&self) -> CharRect {
        todo!("Implement ImplementsToken char_rect")
    }

    fn to_rust(&self) -> TokenStream {
        todo!("Implement ImplementsToken to_rust")
    }
}

struct OverridesToken {

}

impl Token for OverridesToken {
    fn parse(parser: &mut Parser) -> Result<Self> where Self: Sized {
        todo!("Implement OverridesToken parse")
    }
    
    fn char_rect(&self) -> CharRect {
        todo!("Implement OverridesToken char_rect")
    }

    fn to_rust(&self) -> TokenStream {
        todo!("Implement OverridesToken to_rust")
    }
}

struct VirtualsToken {
    fields: ListToken<VirtualToken, '[', ']'>,
    char_rect: CharRect,
}

impl Token for VirtualsToken {
    fn parse(parser: &mut Parser) -> Result<Self> where Self: Sized {
        println!("Virtuals");

        let start = parser.current_pos();
        let end = parser.current_pos();
        parser.expect_char(&['='])?;
        Ok(VirtualsToken {
            fields: ListToken::parse(parser)?,
            char_rect: CharRect::from((start, end)),
        })
    }
    
    fn char_rect(&self) -> CharRect {
        self.char_rect
    }

    fn to_rust(&self) -> TokenStream {
        todo!("Implement VirtualsToken to_rust")
    }
}

struct VirtualToken {
    index: u32,
    function: FunctionToken,
    char_rect: CharRect,
}

impl Token for VirtualToken {
    fn parse(parser: &mut Parser) -> Result<Self> where Self: Sized {
        println!("Virtual");

        let start = parser.current_pos();

        let index = parser.expect_num::<u32>()?;
        parser.expect_char(&['='])?;
        let function = FunctionToken::parse(parser)?;

        function.print();
        let end = parser.current_pos();
        Ok(Self {
            index,
            function,
            char_rect: CharRect::from((start, end)),
        })
    }

    fn char_rect(&self) -> CharRect {
        self.char_rect
    }

    fn to_rust(&self) -> TokenStream {
        todo!("Implement VirtualToken to_rust")
    }
}

struct FunctionToken {
    name: String,
    conv: CallConv,
    params: ListToken<VariableToken, '(', ')'>,
    ret_type: TypeToken,
    char_rect: CharRect,
}

#[derive(Debug)]
enum CallConv {
    Cdecl,
    Stdcall,
    Fastcall,
    Thiscall,
}

impl FunctionToken {
    fn print(&self) {
        print!("{}: {:?} (", self.name, self.conv);

        let mut it = self.params.items.iter().peekable();
        while let Some(param) = it.next() {
            print!("{}: ", param.name);
            param.r#type.print();
            if it.peek().is_some() {
                print!(", ");
            }
        }

        print!(") ");
        self.ret_type.print();
        println!("");
    }
}

impl Token for FunctionToken {
    fn parse(parser: &mut Parser) -> Result<Self> where Self: Sized {
        println!("Function");

        let start = parser.current_pos();

        let name = parser.word();
        parser.expect_char(&[':'])?;

        let conv = parser.word();
        let conv = match conv.as_str() {
            "cdecl" => CallConv::Cdecl,
            "stdcall" => CallConv::Stdcall,
            "fastcall" => CallConv::Fastcall,
            "thiscall" => CallConv::Thiscall,
            conv => panic!("{conv} is not a supported calling convention"),
        };

        let params = ListToken::parse(parser)?;

        let ret_type = TypeToken::parse(parser)?;

        let end = parser.current_pos();
        Ok(Self {
            name,
            conv,
            params,
            ret_type,
            char_rect: CharRect::from((start, end)),
        })
    }

    fn char_rect(&self) -> CharRect {
        self.char_rect
    }

    fn to_rust(&self) -> TokenStream {
        todo!("Implement FunctionToken to_rust")
    }
}

fn parse_bb_bindings(root: &str) -> Vec<Result<ConcreteFileToken>> {
    let root = format!("{}/src/{}", env::var("CARGO_MANIFEST_DIR").unwrap(), root);
    walk_all_bb_files(&Path::new(&root), &|file| {
        println!("file: {}.bb", file.name);
        let mut parser = Parser::new(&file.name, &file.contents);

        Ok(match parser.expect_word(&["class", "struct", "enum", "functions"])?.as_str() { 
            "class" => ConcreteFileToken::ClassToken(ClassToken::parse(&mut parser)?),
            file_type => todo!("Implement keyword: {}", file_type),
        })
    })
}

fn walk_all_bb_files(dir: &Path, action: &impl Fn(File) -> Result<ConcreteFileToken>) -> Vec<Result<ConcreteFileToken>> {
    let mut file_tokens = Vec::new();
    if !dir.is_dir() {
        eprintln!("{} is not a directory", dir.display());
        return file_tokens;
    }

    for entry in match dir.read_dir() {
        Ok(read_dir) => read_dir,
        Err(e) => {
            eprintln!("Failed to read contents from {}: {}", dir.display(), e);
            return file_tokens;
        }
    } {
        let entry = match entry {
            Ok(entry) => entry,
            Err(e) => {
                eprintln!("Failed to read entry from {}: {}", dir.display(), e);
                continue;
            }
        }.path();

        if entry.is_dir() {
            walk_all_bb_files(&entry, action);
        }
        else if entry.extension() == Some(&OsStr::new("bb")) {
            let name = match entry.file_stem() {
                Some(name) => name,
                None => {
                    eprintln!("Failed to get file name from {}", entry.display());
                    continue;
                }
            }.to_string_lossy().to_string();
            let contents = match fs::read_to_string(&entry) {
                Ok(contents) => contents,
                Err(e) => {
                    eprintln!("Failed to read contents from {}: {}", entry.display(), e);
                    continue;
                }
            };
            file_tokens.push(action(File { name, contents }));
        }
    }
    file_tokens
}
