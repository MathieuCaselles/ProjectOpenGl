#pragma once
#include <memory>
#include "../Typelist.h"

namespace Tools {

    template<typename ValidTypes_, bool AllowIfIsBaseOf = false, template<class...> class SmartPtrType = std::unique_ptr>
    class Factory
    {
    public:
        using ValidTypes = ValidTypes_;

        template<typename T>
        using SmartPtr = SmartPtrType<T>;

        template<typename Type, typename... Args>
        static SmartPtr<Type> create(Args... args)
        {
            return SmartPtr<typename Creator<Type>::type>(
                new typename Creator<Type>::type(args...));
        };

    private:
        template<typename Type>
        struct Creator
        {
            static_assert(AllowIfIsBaseOf ? contains_base_of_v<ValidTypes, Type> : contains_v<ValidTypes, Type>);
            using type = Type;
        };
    };

}