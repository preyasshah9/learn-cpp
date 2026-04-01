/*
This is an example of the strategy design pattern.

Strategy defines how something needs to be done.
- Compared to Command design pattern, it does not dictate what needs to be done. Command
design pattern is especially mandatory for undo operations.
- Unlike Visitor design pattern, it is Open for types, but closed for operations.

Let's implement the Strategy pattern using two classes, Square and Circle which needs to provide
Drawable behavior using draw(...args). We will implement inheritance based solution
and also show Value based implementation using `std::function`.
*/

#include <functional>

