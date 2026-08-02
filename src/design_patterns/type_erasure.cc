#include <string>
#include <memory>
#include <iostream>
#include <utility>

namespace details
{
    // Type-erased interface. Logger only depends on this virtual contract,
    // while concrete logger implementations remain hidden.
    class LoggerConcept
    {
    public:
        virtual ~LoggerConcept() = default;
        virtual void log(std::string_view message) const = 0;
        virtual std::unique_ptr<LoggerConcept> clone() const = 0;
    };

    template <typename T>
    class LoggerModel : public LoggerConcept
    {
    public:
        // Perfect forwarding preserves whether the original object should
        // be copied from an lvalue or moved from an rvalue.
        template <typename U>
        LoggerModel(U&& t)
            : obj(std::forward<U>(t))
        {}

        void log(std::string_view message) const override
        {
            obj.log(message);
        }

        std::unique_ptr<LoggerConcept> clone() const override
        {
            static_assert(
                std::is_copy_constructible_v<T>,
                "Logger type must be copy constructible to support clone()"
            );
            return std::make_unique<LoggerModel<T>>(obj);
        }

    private:
        T obj;
    };
};

/*
 * Example of type erasure in C++:
 *
 * Logger can wrap any concrete type that provides:
 *
 *     void log(std::string_view)
 *
 * without requiring inheritance from a common base class.
 *
 * The concrete logger type is erased behind LoggerConcept,
 * allowing Logger to provide a stable runtime interface.
 */
class Logger
{
public:
    template <typename T>
    Logger(T&& t)
        : concept(
            std::make_unique<details::LoggerModel<std::decay_t<T>>>(
                std::forward<T>(t)
            ))
    {}
    void log(std::string_view msg) const
    {
        concept->log(msg);
    }

private:
    std::unique_ptr<details::LoggerConcept> concept;
};