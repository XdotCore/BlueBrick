#include "ColorBase.hpp"

namespace BlueBrick {

	class ColorNone final : public ColorBase {
	public:
		std::string Start() const override;

		static const ColorNone& None();
	};

}
