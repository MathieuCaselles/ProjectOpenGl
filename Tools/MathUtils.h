#include <math.h>

namespace Tools {
    template<typename Type>
    struct Vector3
    { 		
		static_assert(std::is_arithmetic_v<Type>, "Type must be an arithmetic type.");

		Vector3() :X(0), Y(0), Z(0) {};
        Vector3(Type x, Type y, Type z) :X(x), Y(y), Z(z) {};

		Vector3 operator/(Type value) const
		{
			return Vector3(X / value, Y / value, Z / value);
		}

		Type X;
		Type Y;
		Type Z;
	};
}


