# Scheme

Это компактный интерпретатор Scheme на C++23. Интерфейс принимает выражение в виде строки и возвращает результат в виде строки. В репозитории доступны REPL и набор тестов.

## Возможности

- Типы: числа, булевы значения, символы, пары, списки, пустой список.
- Специальные формы: quote, if, lambda, define, set!, and, or.
- Логика и предикаты: boolean?, symbol?, pair?, null?, list?, not.
- Числа: number?, +, -, *, /, =, <, >, <=, >=, max, min, abs.
- Списки: cons, list, car, cdr, set-car!, set-cdr!, list-ref, list-tail.

## Структура репозитория

- scheme/runtime. Базовые типы, ошибки, окружение, утилиты списков.
- scheme/reader. Tokenizer и parser, превращают ввод в AST.
- scheme/eval. Evaluator, процедуры и специальный синтаксис.
- scheme/stdlib. Регистрация встроенных функций и операций.
- scheme/io. Печать объектов в текстовый вид.
- apps/repl. Консольный REPL.
- tests. Тесты на Catch2.
- utils. Небольшие общие хедеры.
- cmake. CMake модули.

## Сборка

### Требования

- CMake 3.25 или новее.
- Компилятор с поддержкой C++23.
- Интернет нужен при первой конфигурации, чтобы CMake скачал Catch2.

### Шаги

1. cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
2. cmake --build build -j 8

### Запуск REPL

- ./build/scheme-repl

### Запуск тестов

- ./build/test_scheme
- ./build/test_scheme_parser
- ./build/test_scheme_tokenizer
