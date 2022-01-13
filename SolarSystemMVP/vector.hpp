//Если вы подключаете вместе с SFML, то тольк после SFML



#ifndef VALUE_VECTOR_HPP
#define VALUE_VECTOR_HPP
#include<cstddef>

namespace Math
{
    template<class ValueType>
    class vector
    {
    public:
        ValueType x,y;
        vector():
            x
            {
                []()
                {
                    if constexpr (std::is_arithmetic_v<ValueType>)
                    {
                        return 0;
                    }
                    else
                    {
                        return ValueType();
                    }
                }()
             },
            y
            {
              []()
                {
                    if constexpr (std::is_arithmetic_v<ValueType>)
                    {
                        return 0;
                    }
                    else
                    {
                        return ValueType();
                    }
                }()
            } {}

        vector(const ValueType& arg_x,const ValueType& arg_y): x{arg_x}, y{arg_y} {}

        template<class Val>
        vector(const vector<Val>& other): x{other.x}, y{other.y} {}

        vector(const vector& other): x{other.x}, y{other.y}{}

        vector(vector&& other)noexcept: x{std::move(other.x)}, y{std::move(other.y)}
        {
            other.x = 0;
            other.y = 0;
        }
        vector& operator=(const vector& other)
        {
            x = other.x;
            y = other.y;
            return *this;
        }
        vector& operator=(vector&& other) noexcept
        {
            x = std::move(other.x);
            y = std::move(other.y);
            other.x = 0;
            other.y = 0;
            return *this;
        }
        ~vector()noexcept = default;


        template<class Val>
        vector& operator=(const vector<Val>& other)
        {
            x = other.x;
            y = other.y;
            return *this;
        }

        template<class Val>
        vector& operator=(vector<Val>&& other)
        {
            x = std::move(other.x);
            y = std::move(other.y);
            return *this;
        }


        vector& operator+=(const vector& rhs)
        {
            x+=rhs.x;
            y+=rhs.y;
            return *this;
        }
        vector& operator-=(const vector& rhs)
        {
            x-=rhs.x;
            y-=rhs.y;
            return *this;
        }
        template<class Scalar>//requires convertable to integral, floatpoint
        vector& operator*=(const Scalar& rhs)
        {
            x*=rhs;
            y*=rhs;
            return *this;
        }

        template<class Scalar>//requires convertable to integral, floatpoint
        vector& operator/=(const Scalar& rhs)
        {
            x/=rhs;
            y/=rhs;
            return *this;
        }
        bool operator==(const vector& rhs)
        {
            return x == rhs.x &&
                   y == rhs.y;
        }
        bool operator!=(const vector& rhs)
        {
            return !((*this)==rhs);
        }

        std::conditional_t<std::is_same_v<ValueType, float>, float, double> abs()
        {
            return sqrt(x * x + y * y);
        }
        
        void normalise()
        {
            (*this) /= abs();
        }


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //scalarProduct
        template<class VecT, class VecY>
        friend auto operator*(const VecT& lhs,const VecY& rhs) -> decltype(lhs.x+rhs.y)
        {
            return lhs.x*rhs.x + lhs.y*rhs.y;
        }

        template<class Scalar>
        friend vector operator/(const vector& lhs, const Scalar& rhs)
        {
            auto res = lhs;
            res/=rhs;
            return res;
        }
        template<class Scalar>
        friend vector operator*(const vector& lhs, const Scalar& rhs)
        {
            auto res = lhs;
            res*=rhs;
            return res;
        }

        template<class Scalar>
        friend vector operator+(const vector& lhs, const Scalar& rhs)
        {
            auto res = lhs;
            res+=rhs;
            return res;
        }

        template<class Scalar>
        friend vector operator-(const vector& lhs, const Scalar& rhs)
        {
            auto res = lhs;
            res-=rhs;
            return res;
        }

        friend vector operator-(const vector& lhs)
        {
            auto res = lhs;
            res*=-1;
            return res;
        }


        #ifdef SFML_VECTOR2_HPP
        vector(const sf::Vector2<ValueType>& other) : x{other.x}, y{other.y} {}

        template<class U>
        operator sf::Vector2<U>() const
        {
            return sf::Vector2<U>(x, y);
        }

        #endif


    };
}

using vector2di = typename Math::vector<int>;
using vector2df = typename Math::vector<float>;


#endif/*VALUE_VECTOR_HPP*/