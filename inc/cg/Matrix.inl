#if defined(MATRIX_HPP)

namespace cg
{

template <uint8_t D>
square_matrix<D>::square_matrix(double identity) : val_ptr(new double[D * D]())
{
    *this = identity;
}

template <uint8_t D>
square_matrix<D>::square_matrix(const square_matrix<D> &other) : val_ptr(new double[D * D]())
{
    *this = other;
}

template <uint8_t D>
square_matrix<D>::square_matrix(square_matrix<D> &&other) : val_ptr(nullptr)
{
    *this = std::move(other);
}

template <uint8_t D>
square_matrix<D>::~square_matrix()
{
    delete[] this->val_ptr;
}

template <uint8_t D>
square_matrix<D> & square_matrix<D>::operator=(double identity)
{
    //this->clear();
    for(uint8_t i = 0; i < D; ++i)
    {
        this->at(i,i) = identity;
    }
    //std::cerr << (*this) << "\n";
    return *this;
}

template <uint8_t D>
square_matrix<D> & square_matrix<D>::operator=(const square_matrix<D> &other)
{
    if(&other != this)
    {
        //this->clear();
        for(uint8_t i = 0; i < (D * D); ++i)
        {
            this->operator[](i) = other[i];
        }
    }
    //std::cerr << (*this) << "\n";
    return *this;
}

template <uint8_t D>
square_matrix<D> & square_matrix<D>::operator=(square_matrix<D> &&other)
{
    if(this != &other)
    {
        delete[] this->val_ptr;
        this->val_ptr = other.val_ptr;
        other.val_ptr = nullptr;
    }

    return *this;
}

template <uint8_t D>
const double * square_matrix<D>::raw() const {return val_ptr;}

template <uint8_t D>
double & square_matrix<D>::operator[](uint8_t index)
{
    if(index > (D * D - 1) or index < 0)
        throw err::exception("Matrix out of range at 'operator[](uint8_t)'");
    return val_ptr[index];
}

template <uint8_t D>
const double & square_matrix<D>::operator[](uint8_t index) const
{
    if(index > (D * D - 1) or index < 0)
        throw err::exception("Matrix out of range at 'operator[](uint8_t) const'");
    return val_ptr[index];
}

template <uint8_t D>
double & square_matrix<D>::at(uint8_t col, uint8_t row)
{
    if(col > (D - 1) or row > (D - 1) or col < 0 or row < 0)
        throw err::exception("Matrix out of range at 'at(uint8_t)'");
    return val_ptr[(col * D) + row];
}

template <uint8_t D>
const double & square_matrix<D>::at(uint8_t col, uint8_t row) const
{
    if(col > (D - 1) or row > (D - 1) or col < 0 or row < 0)
        throw err::exception("Matrix out of range at 'at(uint8_t) const'");
    return val_ptr[(col * D) + row];
}

template <uint8_t D>
uint8_t square_matrix<D>::dimension() const {return D;}

template <uint8_t D>
uint16_t square_matrix<D>::size() const {return D * D;}

template <uint8_t D>
square_matrix<D> square_matrix<D>::transposed() const
{
    square_matrix<D> tmp;
    
    for(uint8_t col = 0; col < D; ++col)
    {
        for(uint8_t row = 0; row < D; ++row)
        {
            tmp.at(row, col) = this->at(col, row);
        }
    }
    return tmp;
}

template <uint8_t D>
square_matrix<D> square_matrix<D>::operator*(const square_matrix &other) const
{
    square_matrix<D> tmp = *this;

    tmp *= other;

    return tmp;
}

template <uint8_t D>
square_matrix<D> & square_matrix<D>::operator*=(const square_matrix<D> &other)
{
    // make an empty matrix to hold result
    square_matrix<D> tmp(0.0);

    for(uint8_t i = 0; i < D; ++i)
    {
        for(uint8_t j = 0; j < D; ++j)
        {
            for(uint8_t k = 0; k < D; ++k)
            {
                //tmp.at(i, j) += this->at(i, k) * other.at(k, j); // original
                tmp.at(i, j) += this->at(k, j) * other.at(i, k);
            }
        }
    }
    //std::cerr << "Result of Matrix multiply is: \n" ;
    //std::cerr << *this << "     x\n" << other << "     =\n"<< tmp << std::endl;

    *this = std::move(tmp); // Use move semantics to keep this cheap;
    return *this;
}

template <uint8_t D>
void square_matrix<D>::clear(void)
{
    for(uint8_t i = 0; i < (D * D); ++i)
    {
        this->operator[](i) = 0.0;
    }
}

} // End namespace cg

template <uint8_t D>
std::ostream & operator<<(std::ostream &out, const cg::square_matrix<D> &m)
{
    for(uint8_t col=0; col < D; ++col)
    {
        out << "[ ";
        for(uint8_t row=0; row < D; ++row)
        {
            out << m.at(row, col) << " ";
        }
        out << "]\n";
    }
    return out;
}

#endif // end include guard

