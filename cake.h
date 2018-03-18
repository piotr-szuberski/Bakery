#ifndef CAKE_H
#define CAKE_H

#include <type_traits>
#include <cassert>

//obliczanie ln2 za pomoca serii Taylora
constexpr double ComputeLn2(int n)
{
    double ln2 = 0;
    double x = 1./3;
    double pow_x = 1./3;

    for(int k=1; k<n; k+=2)
    {
        ln2 += pow_x / k;
        pow_x *= x*x;
    }
    return ln2 * 2;
}

constexpr double ln2 = ComputeLn2(14);

// szablon klasy Cake
// T - typ calkowitoliczbowy
// P - typ zmiennoprzecinkowy
// isApplePie - okresla, czy mozna wykonac na produkcie restock
// isForSale - okresla, czy produkt jest na sprzedaz
template <typename T, T length, T width,
        typename P = float, bool isApplePie = false, bool isForSale = false>
class Cake
{
    int stock_;
    P price_;

    static_assert(std::is_integral<T>::value, "Bok musi byc calkowity\n");
    static_assert(std::is_floating_point<P>::value,
            "Cena musi byc zmiennoprzecinkowa\n");

public:
    using SizeType = T;
    using PriceType = P;

    //test na bycie ApplePie
    using ApplePieTest = typename std::conditional<isApplePie,
            std::true_type, std::false_type>::type;

    using CheckIfForSale = typename std::conditional<isForSale,
            std::true_type, std::false_type>::type;

    //wywoluje konstruktor CreamCake gdy P rzeczywiste i T jest calkowite
    template<bool checkType = isForSale,
            typename = typename std::enable_if<checkType>::type>
    Cake(int initial_stock, P price) : stock_(initial_stock), price_(price)
    {
        assert(initial_stock > 0);
    }

    //wywoluje konstuktor CheeseCake gdy P domyslne (int) i T jest calkowite
    template<bool checkType = isForSale,
            typename = typename std::enable_if<!checkType>::type>
    Cake(int initial_stock) : stock_(initial_stock)
    {
        assert(initial_stock > 0);
    }


    static constexpr double getArea()
    {
        return length * width * ln2;
    }

    int getStock()
    {
        return stock_;
    }

    //udostepnia funkcje tylko gdy P jest rzeczywiste
    template<bool checkType = isForSale,
            typename = typename std::enable_if<checkType>::type>
    void sell()
    {
        if(stock_ > 0)
            stock_--;
    }

    //udostepnia funkcje tylko gdy P jest rzeczywiste
    template<bool checkType = isForSale,
            typename = typename std::enable_if<checkType>::type>
    P getPrice() const
    {
        return price_;
    }
};

template <typename T, T length, T width>
using CheeseCake = Cake<T, length, width, float, false, false>;

template <typename T, T length, T width, typename P>
using CreamCake = Cake<T, length, width, P, false, true>;


#endif //CAKE_H
