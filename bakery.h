#ifndef BAKERY_H
#define BAKERY_H

#include <tuple>

// szablon klasy piekarni
// C - typ zmiennoprzecinkowy
// A - typ calkowitoliczbowy
// typy P musza byc unikalne
// parametr shelfArea nie moze byc mniejszy od lacznej powierzchni produktow
template<typename C, typename A, A shelfArea, typename... P>
class Bakery
{
    // sprawdzenie czy przedmiot jest na sprzedaz
    template<typename Me>
    static constexpr bool isForSale =
            std::is_same<typename Me::CheckIfForSale, std::true_type>::value;

    // sprawdzenie czy C jest typem zmiennoprzecinkowym
    static_assert(std::is_floating_point<C>::value,
        "C nie jest typem zmiennoprzecinkowym\n");

    // sprawdzenie czy A jest typem całkowitym
    static_assert(std::is_integral<A>::value,
        "A nie jest typem całkowitym\n");

    // suma powierzchni produktow
    template<typename... Args>
    static constexpr A sumArea = (... + Args::getArea());

    // sprawdzenie zgodnosci typow wymiaru
    template<typename... args>
    static constexpr bool checkA =
        (... && (std::is_same<typename args::SizeType, A>::value));

    // sprawdzenie zgodnosci typow ceny
    template<typename... args>
    static constexpr bool checkC = (... && (!isForSale<args> ||
        std::is_same<typename args::PriceType,C>::value));

    // sprawdzanie duplikatów
    template<typename Me, typename... Args>
    static constexpr int countMyDupes()
    {
        return (... + (std::is_same<Me,Args>::value));
    };

    template<typename... args>
    static constexpr bool checkDupes =
        (... && (countMyDupes<args,args...>() == 1));

    // sprawdzenie czy produkt jest w products_
    template<typename Me, typename... Args>
    static constexpr bool isInStock()
    {
        return (... || (std::is_same<Me,Args>::value));
    };

    //sprawdzenie czy produkt jest applepie
    template<typename Me>
    static constexpr bool isApplePie =
        std::is_same<typename Me::ApplePieTest, std::true_type>::value;

    static_assert(checkDupes<P...>, "Znaleziono duplikaty produktow\n");
    static_assert(sumArea<P...> <  shelfArea,
        "Powierzchnia produktow wieksza niz powierzchnia polki\n");
    static_assert(checkA<P...>, "Typy wymiarow niezgodne\n");
    static_assert(checkC<P...>, "Typy ceny niezgodne\n");

    // kontener na produkty o roznych typach
    std::tuple<P...> products_;

    C profits_;

public:

    Bakery(const P&... products) : products_(products...), profits_(0) {};

    C getProfits() const
    {
        return profits_;
    }

    // sprzedaje produkt pod warunkiem ze typ produktu znajduje sie w piekarni
    // i jest na sprzedaz. Gdy liczba takiego produktu wynosi 0, to nic nie robi.
    template <typename Product>
    constexpr void sell()
    {
        static_assert(isForSale<Product>, "Produkt nie jest na sprzedaż\n");
        static_assert(isInStock<Product, P...>(), "Produkt niedostepny\n");
        auto product = std::get<Product>(products_);
        if(product.getStock() > 0)
        {
            profits_ += product.getPrice();
            std::get<Product>(products_).sell();
        }
    }

    // podaje ilosc danego produktu jesli takowy znajduje sie w piekarni
    template <typename Product>
    constexpr int getProductStock() const
    {
        static_assert(isInStock<Product, P...>(), "Produkt niedostepny\n");
        auto product = std::get<Product>(products_);
        return product.getStock();
    }

    // zwieksza ilosc ApplePie o wartosc additionalStock (i tylko tego typu)
    template <typename Product>
    constexpr void restock(int additionalStock)
    {
        static_assert(isInStock<Product, P...>(), "Produkt niedostepny\n");
        static_assert(isApplePie<Product>, "Produkt nie jest typu ApplePie\n");
        std::get<Product>(products_).restock(additionalStock);
    }
};

#endif //BAKERY_H
