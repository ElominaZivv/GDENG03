#pragma once

#include <string>

namespace jazzy {

	class Screens
	{
    public:
        explicit Screens(const std::string& name);
        virtual ~Screens() = default;

        virtual void draw() = 0;

        const std::string& getName() const;

    protected:
        std::string screenName;
	};

}

