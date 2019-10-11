#include <array>

/**
 * Base type of vector. 
 */
template<size_t Size, typename Type>
class VectorNT {
protected:
    std::array<Type, Size> m_vector;
};


template<typename Type>
class Vector2T : public VectorNT<2, Type> {
private: // aliases
    using this_type = Vector2T<Type>;

public:
    Vector2T() = default;
    Vector2T(const Type& _x_, const Type& _y_) {
        x(_x_);
        y(_y_);
    }

    const Type& x() {return m_vector[0];}
    const Type& y() {return m_vector[1];}
    void x(Type _x_) {m_vector[0] = _x_;}
    void y(Type _y_) {m_vector[1] = _y_;}

    this_type operator + (const this_type& other) {
        this_type r = *this;
        r.x(r.x() + other.x());
        r.y(r.y() + other.y());
        return r;
    }

    this_type operator - (const this_type& other) {
        this_type r = *this;
        r.x(r.x() - other.x());
        r.y(r.y() - other.y());
        return r;
    }
};

template<typename Type>
class Vector3T : public VectorNT<3, Type> {
private: // aliases
    using this_type = Vector2T<Type>;

public:
    Vector3T() = default;
    Vector3T(const Type& _x_, const Type& _y_, const Type& _z_) {
        x(_x_);
        y(_y_);
        z(_z_);
    }

    const Type& x() {return m_vector[0];}
    const Type& y() {return m_vector[1];}
    const Type& z() {return m_vector[2];}
    void x(Type _x_) {m_vector[0] = _x_;}
    void y(Type _y_) {m_vector[1] = _y_;}
    void z(Type _z_) {m_vector[2] = _z_;}

    this_type operator + (const this_type& other) {
        this_type r = *this;
        r.x(r.x() + other.x());
        r.y(r.y() + other.y());
        r.z(r.z() + other.z());
        return r;
    }

    this_type operator - (const this_type& other) {
        this_type r = *this;
        r.x(r.x() - other.x());
        r.y(r.y() - other.y());
        r.z(r.z() - other.z());
        return r;
    }
};

using Vector2f = Vector2T<float>;
using Vector3f = Vector3T<float>;

template<>
Vector2f::Vector2T() { x(0.f); y(0.f); }

class MatrixNNT {

};