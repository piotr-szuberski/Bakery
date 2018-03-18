#ifndef PIE_H
#define PIE_H

#include <type_traits>
#include <cassert>

//obliczanie pi z przeksztalconego szeregu Leibniza
constexpr double ComputePi(int n)
{
    double pi = 0;

    for(int k=0; k<n; k++)
    {
        pi += ((1.0/(4.0*k+1.0))/(2.0*k+1.0))/(k+1.0);
    }
    return pi * 3;
}

constexpr double pi = ComputePi(777);

// szablon klasy Pie
// R - typ calkowitoliczbowy
// P - typ zmiennoprzecinkowy
// isApplePie - okresla, czy mozna wykonac na produkcie restock
// isForSale - okresla, czy produkt jest na sprzedaz
template <typename R, R radius, typename P = float, bool isApplePie = false, bool isForSale = false>
class Pie
{
    int stock_;
    P price_;

    static_assert(std::is_integral<R>::value, "Promien musi byc calkowity\n");
    static_assert(std::is_floating_point<P>::value,
            "Cena musi byc zmiennoprzecinkowa\n");

public:

    using SizeType = R;
    using PriceType = P;

    //test na bycie typem ApplePie
    using ApplePieTest = typename std::conditional<isApplePie, std::true_type,
            std::false_type>::type;

    using CheckIfForSale =typename std::conditional<isForSale, std::true_type,
            std::false_type>::type;


    //wywoluje konstruktor dla ApplePie, gdy R jest calkowite, a P rzeczywiste
    template<bool checkType = isForSale,
            typename = typename std::enable_if<checkType>::type>
    Pie(int initial_stock, P price) : stock_(initial_stock), price_(price)
    {
        assert(initial_stock > 0);
    }

    //wywoluje konstruktor dla CherryPie, gdy R jest calkowite, a P pozostaje
    //domyslne (int)
    template<bool checkType = isForSale,
        typename = typename std::enable_if<!checkType>::type>
    Pie(int initial_stock) : stock_(initial_stock)
    {
        assert(initial_stock > 0);
    }

    static constexpr double getArea()
    {
        return pi * radius * radius;
    }

    int getStock() const
    {
        return stock_;
    }

    //udostepnia funkcje tylko gdy P jest rzeczywiste
    template<bool checkType = isForSale, typename = typename std::enable_if<checkType>::type>
    void sell()
    {
        if(stock_ > 0)
            stock_--;
    }

    //udostepnia funkcje tylko gdy P jest rzeczywiste
    template<bool checkType = isForSale, typename = typename std::enable_if<checkType>::type>
    P getPrice() const
    {
        return price_;
    }

    //udostepnia funkcje tylko gdy P jest rzeczywiste
    template<bool checkType = isForSale, typename = typename std::enable_if<checkType>::type>
    void restock(int additionalStock)
    {
        stock_ += additionalStock;
    }
};

template <typename R, R radius>
using CherryPie = Pie<R, radius, float, false, false>;

template <typename R, R radius, typename P>
using ApplePie = Pie<R, radius, P, true, true>;


#endif //PIE_H
