# Отладка программ с помощью GDB

## Введение

Прежде чем приступить к изучению [GDB](https://www.gnu.org/software/gdb/documentation/) -
мощного инструмента, предназначенного для отладки программ - предлагаю немного
поразмышлять над тем, что же представляет собой процесс отладки.
Рекомендую прочитать довольно короткий пост
["Debugging"](http://airs.com/ian/essays/debug/debug.html) в блоге Иана Ланса
Тейлора, ведущего разработчика из компании Google.

Итак, вы написали программу и знаете, что в ней есть ошибка. Откуда знаете? К
примеру, программа аварийно завершает работу на некоторых входных данных или
выдает результат, противоречащий здравому смыслу. Распространенный, но менее
приятный вариант: вам об этом сообщил пользователь. В любом случае, первый шаг,
про который не следует забывать - нужно постараться добиться устойчивого
воспроизведения проблемы. Если для это требуется некоторые входные данные
(файлы), и есть риск, что программа (или вы сами в процессе отладки) поменяет
их, стоит позаботиться о резервной копии. Возможно, для воспроизведения проблемы
придется воссоздать условия в которых работает программа у пользователя (ОС,
версии библиотек), но к счастью, этого не потребуется в рамках курсовых
проектов.

Следующим шагом будет собственно процесс отладки: нужно найти ошибку в коде,
вызывающую ошибочное поведение программы. Автор курса
["Software Debugging"](https://www.udacity.com/course/viewer#!/c-cs259),
Andreas Zeller предлагает (и мне это кажется вполне разумным) применить в
отладке научный метод: ученый, исследуя явление природы, высказывает гипотезу,
затем пытается с помощью нее предсказать определенные свойства природного
явления, в ходе эксперимента проверяет верно ли предсказание, и либо
корректирует гипотезу, либо использует ее в ходе дальнейших рассуждений. В нашем
случае гипотезами будут предположения о присутствующей в коде ошибке (например о
ее расположении в пределах модуля), а экспериментами будут запуски программы и
наблюдение за ее работой. Как и в научном эксперименте важно четко представлять
начальные условия: какие входные данные и какую версию кода (в процессе отладки
вы, скорее всего, будете менять его) вы сейчас используете, а также иметь
возможность воспроизвести промежуточные результаты (откатиться назад).

Простейший способ наблюдать за поведением программы - это отладочный вывод. Вы
добавляете в программу код, который выводит данные о внутреннем состоянии
программы (как правило, значения перемененных) на консоль или в файл (или даже
используете специализированные средства для ведения логов) и затем
просматриваете их, пытаясь подтвердить или опровергнуть свою гипотезу.

## Отладочный вывод

При выводе данных в лог часто хочется понять, в какой именно точке программы
сейчас находится управление. Простой, но не слишком продуктивный способ:
написать что-то вроде

```C++
std::cout << "HERE! x = " << x << '\n';
```

Ну а дальше придется писать `"HERE1"`, `"!!!HERE2!!!"`, в которых будет легко
запутаться. Полезной может оказаться функция наподобие следующей:

```C++
#include <iostream>
#include <string>
#include <sstream>

std::string loc(const char *func = __builtin_FUNCTION(),
    const char *file = __builtin_FILE(), int line = __builtin_LINE())
{
    std::ostringstream res;
    res << " (" << func << " in " << file << ":" << line << ")\n";
    return res.str();
}
```

Использование:

```C++
std::cout << "x = " << x << loc();
```

В результате в отладочном выводе будет присутствовать имя функции, номер строки и
имя файла.

`__builtin_FUNCTION` (а также FILE и LINE) появились в GCC 4.8, поэтому с более
старой версией GCC (а также с другими компиляторами) это может не сработать.
Возможно в следующем стандарте C++ появятся штатные средства для получения
позиции в исходном коде, соответствующее предложение
[N4419](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4419.pdf)
рассматривается комитетом по стандартизации C++. Пока же, если `__builtin_FUNCTION`
не подходит, придется использовать макросы (`__FILE__` и `__LINE__` - также
расширения компилятора, но более распространенные). Аналогичный по
функциональности фрагмент кода, использующий макросы и написанный на чистом C:

```C
#include <stdio.h>

#define log(s, ...) printf(s " (%s in %s:%d)\n", __VA_ARGS__, __func__, \
    __FILE__, __LINE__)
```

Использование:

```C
log("x = %d", x);
```

Ну а еще более правильный (но и затратный по времени) способ - разобраться с
какой-нибудь библиотекой для логирования, например
[Boost.Log](http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/index.html)
или [glog](https://github.com/google/glog) от Google.

## Описание примера

Пример: [calc.cpp](examples_gdb/calc.cpp).

В качестве примера я подготовил небольшую программу. Это калькулятор, который
умеет выполнять 3 арифметических действия над целыми числами: сложение,
вычитание и умножение. Он поддерживает выражения произвольной длины, но без
скобок. Выражение подается в поток `stdin`, а результат печатается в `stdout`.
Калькулятор выполняет разбор выражения и вычисление значения при помощи
рекурсивного спуска (этот алгоритм должен быть вам хорошо знаком). В случае
если на вход подано некорректное выражение, выдается сообщение об ошибке ("ERROR.
Unexpected end of input" либо "ERROR. Invalid expression").

Функция `eval_primary` отвечает за разбор одного числа, `eval_term` -
произведения одного или нескольких сомножителей, а `eval_expr` - всего
выражения. Для чтения из `stdin` используются функции `peek_not_eof` (вернуть
следующий символ либо 0 в случае окончания ввода) и `drop_char` (перейти к
следующему символу).

## Подготовка программы к отладке

Как и в случае с Valgrind для запуска программы под GDB следует собрать ее с 
отладочной информацией. В случае с GDB это более критично, чем с Valgrind.

Используем следующую команду:

    $ g++ -ggdb3 -o calc calc.cpp

Попробуем вычислить несколько выражений:

    $ echo "1+2*3" | ./calc
      7.000000

    $ echo "1+*2" | ./calc
    ERROR. Unexpected end of input

Но почему "Unexpected end of input" не "Invalid expression"? Попробуем
разобраться.

## Запуск под отладчиком

Для начала, сохраним выражение, на котором мы получили неверный результат в
файл:

    $ echo "1+*2" > expr.txt

Далее, запустим калькулятор под отладчиком:

    $ gdb calc

Если бы программа принимала на вход дополнительные аргументы командной строки,
их можно было бы указать следующим образом:

    $ gdb --args some-program arg1 arg2

После запуска GDB загружает программу в память (но не запускает ее) и  выводит
приглашение командной строки:

    (gdb)

Наше взаимодействие с отладчиком сводится к вводу команд и анализу результата их
исполнения. Работа в отладчике напоминает работу с командной оболочкой, такой
как bash. Например, стрелками "вверх/вниз" осуществляется навигация по ранее
введенным командам, а клавиша `Tab` служит для автодополнения.

Чтобы запустить программу, нужно ввести команду
[run](https://sourceware.org/gdb/current/onlinedocs/gdb/Starting.html).
Почти любую команду можно сокращать, как правило сокращенное имя команды - это
первая буква в названии (например, `r` для команды `run`). В качестве аргументов
можно указать, какие потоки ввода-вывода и как мы хотим перенаправить. В данном
случае, мы хотим перенаправить файл `expr.txt` в `stdin`:

    (gdb) r < expr.txt
    Starting program: /home/miyuki/projects/examples_gdb/calc < expr.txt
    ERROR. Unexpected end of input
    [Inferior 1 (process 12894) exited normally]

Выводится полный путь к программе, программа исполняется (мы видим результат
выполнения). После того как программа завершилась, отладчик выводит информацию о
том, с каким результатом (кодом возврата или сигналом) был завершен процесс.
Термином "inferior" называется отлаживаемая программа (в некотором смысле, это
жаргон, устоявшийся среди авторов GDB).

Мы можем повторить запуск, введя `run` еще раз. Кстати, это можно сделать и в
процессе отладки (до завершения программы), в этом случае выполнение будет
прервано и процесс будет запущен "с чистого листа".

## Точки останова

Одна из частых задач в отладке - остановить программу, когда поток управления
достигнет некоторой точки в программе (например, стоки в файле или определенной
функции). Эта задача решается при помощи точек останова (breakpoint).

Для создания точек останова служит команда
[break](https://sourceware.org/gdb/current/onlinedocs/gdb/Set-Breaks.html#Set-Breaks)
(или сокращённо `b`). Попробуем воспользоваться этой командой:

    $ gdb calc
    (gdb) break eval_expr
    Breakpoint 1 at 0x400940: file calc.cpp, line 75.
    (gdb) b eval_term
    Breakpoint 2 at 0x400892: file calc.cpp, line 52.
    (gdb) b calc.cpp:26
    Breakpoint 3 at 0x4007a0: file calc.cpp, line 26.

Здесь я установил три точки останова, две из которых заданы при помощи имен
функций, а последняя - при помощи имени файла и номера строки (в процессе
отладки не помешает иметь под рукой исходный код, открытый в редакторе). Строка
26 находятся в начале функции `eval_primary`.

Запустим программу:

    (gdb) run < expr.txt
    Starting program: /home/miyuki/projects/examples_gdb/calc < expr.txt

    Breakpoint 1, eval_expr (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe290) at calc.cpp:75
    75          retcode_t rc = eval_term(in, &term);

Как видим, выполнение прервалось в начале функции `eval_expr`. Для продолжения
используем команду
[continue](https://sourceware.org/gdb/current/onlinedocs/gdb/Continuing-and-Stepping.html)
(сокращенно `c`):

    (gdb) c
    Continuing.

    Breakpoint 2, eval_term (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe258) at calc.cpp:52
    52          retcode_t rc = eval_primary(in, &prim);
    (gdb)
    Continuing.

    Breakpoint 3, eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:26
    26          bool negate = false;

Обратите внимание, что если просто нажать "Ввод", не вводя никакой команды, то
будет повторно выполнена предыдущая команда.

Команда [info break](https://sourceware.org/gdb/current/onlinedocs/gdb/Set-Breaks.html)
выводит список точек останова:

    (gdb) info break
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x0000000000400940 in eval_expr(_IO_FILE*, double*) at calc.cpp:75
            breakpoint already hit 1 time
    2       breakpoint     keep y   0x0000000000400892 in eval_term(_IO_FILE*, double*) at calc.cpp:52
            breakpoint already hit 1 time
    3       breakpoint     keep y   0x00000000004007a0 in eval_primary(_IO_FILE*, double*) at calc.cpp:26
            breakpoint already hit 1 time

Команды
[disable](https://sourceware.org/gdb/current/onlinedocs/gdb/Disabling.html)
и `enable` позволяют временно "отключать" и снова "включать" точки останова.
[delete](https://sourceware.org/gdb/current/onlinedocs/gdb/Delete-Breaks.html)
(или `del`) удаляет точку останова:

    (gdb) disable 1
    (gdb) del 2
    (gdb) del 3
    (gdb) info break
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep n   0x0000000000400940 in eval_expr(_IO_FILE*,double*) at calc.cpp:75
            breakpoint already hit 1 time

## Просмотр исходного кода программы в отладчике

Предположим теперь, что мы хотим выполнить программу строчка за строчкой и
проанализировать, как работает функция `eval_primary`. При пошаговом выполнении
хотелось бы иметь перед глазами код программы. Можно просто открыть редактор в
соседнем окне, но существуют и способы просмотра исходного кода в самом
отладчике.

Команда [list](https://sourceware.org/gdb/current/onlinedocs/gdb/List.html)
(или просто `l`) выводит 10 строк кода (количество настраивается,
подробности см. в документации к GDB) вокруг текущей позиции. Повторное
выполнение команды приведет к выводу следующих 10 строк.

Альтернативой служит режим
[TUI](https://sourceware.org/gdb/current/onlinedocs/gdb/TUI.html)
(Text User Interface), в котором листинг программы и команды выводятся в
отдельных текстовых окнах. К сожалению, этот режим не до конца отлажен
(отлаживать отладчик видимо не так-то просто :) ) и в нем возможны баги с
отображением, но проявляются они редко. Для перехода в режим TUI (и выхода из
него) служит комбинация клавиш `Ctrl+x, a`.

Комбинацией `Ctrl+x, o` фокус ввода можно переключать между окном команд и
окном с листингом программы. Когда фокус находится в окне с листингом, по нему
можно осуществлять навигацию стрелками и клавишами `Page Up/Page Down`.

## Пошаговое выполнение программы

Для выполнения кода, написанного на одной строке служит команда
[step](https://sourceware.org/gdb/current/onlinedocs/gdb/Continuing-and-Stepping.html)
(или `s`):

    Breakpoint 3, eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:26
    26          bool negate = false;
    (gdb) s
    27          while (char sgn = peek_not_eof(in)) {
    (gdb) s
    peek_not_eof (in=0x7ffff75b54e0 <_IO_2_1_stdin_>) at calc.cpp:6
    6           char c = std::getc(in);
    (gdb) s
    7           if (std::feof(in) || c == '\n')


Особенностью этой команды является то, что она заходит внутрь вызываемых
функций, но только тех, для которых есть отладочная информация, поэтому мы зашли
внутрь `peek_not_eof`, но не зашли в `std::getc`.

Если же нам не требовалось заходить в функцию `peek_not_eof`, следовало бы
использовать команду
[next](https://sourceware.org/gdb/current/onlinedocs/gdb/Continuing-and-Stepping.html)
(или `n`). В этом случае сеанс отладки выглядел бы так:

    Breakpoint 3, eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:26
    26          bool negate = false;
    (gdb) n
    27          while (char sgn = peek_not_eof(in)) {
    (gdb) n
    28              if (sgn != '-')

## Вывод значений переменных

Помимо знания того, какие участки программы и в каком порядке были выполнены,
для отладки также важно знать какие значения принимали переменные. Для вывода
значений перменных служит команда
[print](https://sourceware.org/gdb/current/onlinedocs/gdb/Data.html)
(или `p`). Продолжим предыдущий пример. Так можно просмотреть значение
переменной `sgn`:

    (gdb) p sgn
    $1 = 49 '1'

Команда `print` может выводить значения в разных
[форматах](https://sourceware.org/gdb/current/onlinedocs/gdb/Output-Formats.html).
Например, модификатор `/x` позволяет вывести значение в шестнадцатеричной
системе счисления:

    (gdb) p/x sgn
    $2 = 0x31

Команда
[info locals](https://sourceware.org/gdb/current/onlinedocs/gdb/Frame-Info.html)
выведет значения всех локальных переменных:

    (gdb) info locals
    sgn = 49 '1'
    negate = false
    seen_digit = false
    cur_val = 0

## Перемещение по стеку вызовов

Находясь в любой точке программы, можно посмотреть, какая именно
последовательность вложенных вызовов функций привела к ней. Для этого
используется команда
[backtrace](https://sourceware.org/gdb/current/onlinedocs/gdb/Backtrace.html)
(или `bt`):

    (gdb) bt
    #0  eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:28
    #1  0x00000000004008a5 in eval_term (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe258) at calc.cpp:52
    #2  0x0000000000400953 in eval_expr (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe290) at calc.cpp:75
    #3  0x0000000000400a2c in main () at calc.cpp:102

С помощью команд
[up](https://sourceware.org/gdb/current/onlinedocs/gdb/Selection.html)
и `down` можно перемещаться вверх и вниз по стеку вызовов, а команда
[frame](https://sourceware.org/gdb/current/onlinedocs/gdb/Selection.html)
(сокращённо `f`) позволяет перейти к кадру заданным номером (нумерация
начиначется с 0 от наиболее вложенного кадра):

    (gdb) up
    #1  0x00000000004008a5 in eval_term (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe258) at calc.cpp:52
    52          retcode_t rc = eval_primary(in, &prim);
    (gdb) frame 3
    #3  0x0000000000400a2c in main () at calc.cpp:102
    102         retcode_t rc = eval_expr(stdin, &result);

Находясь в любом кадре, вы можете просматривать значения локальных переменных.
Обратите внимание, что переход по кадрам никак не влияет на исполнение программы
(вы просто анализируете состояние в котором находится программа).

## Пример сеанса отладки

Как вы помните, в разделе "Подготовка программы к отладке" было показано
проявление ошибки в программе: на выражение "1+*2" программа выдаёт результат
"Unexpected end of input" вместо ожидаемого "Invalid expression". Подумаем,
почему так может произойти.

Сообщение выводится, если функция `eval_expr` возвращает константу `E_EOF`.
Помимо функции `main` и объявления, она встречается в программе всего один раз:
в строке 44. Проверим, действительно ли эта строка приводит к тому, что `E_EOF`
оказывается в `main`. Для этого выставим точку останова и запустим программу:

    (gdb) b 44
    Breakpoint 1 at 0x40084b: file calc.cpp, line 44.
    (gdb) r < expr.txt
    Starting program: /home/miyuki/projects/examples_gdb/calc < expr.txt

    Breakpoint 1, eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:44
    44              return E_EOF;

Посмотрим, как это значение обрабатывается дальше:

    (gdb) n
    47      }
    (gdb) n
    eval_term (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe258) at calc.cpp:53
    53          if (rc != E_OK)
    (gdb) n
    54              return rc;
    (gdb) n
    70      }
    (gdb) n
    eval_expr (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe290) at calc.cpp:86
    86              if (rc != E_OK)
    (gdb) n
    87                  return rc;
    (gdb) n
    97      }
    (gdb) n
    main () at calc.cpp:103
    103         switch (rc) {

Предположение оказалось верным: код возврата передаётся вверх по стеку и
попадает в `main`. Запустим программу ещё раз и попытаемся понять, как именно мы
попадаем в это точку.

    Breakpoint 1, eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:44
    44              return E_EOF;
    (gdb) bt
    #0  eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:44
    #1  0x00000000004008a5 in eval_term (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe258) at calc.cpp:52
    #2  0x00000000004009b3 in eval_expr (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe290) at calc.cpp:85
    #3  0x0000000000400a2c in main () at calc.cpp:102

Строка 52 соответствует первому вызому `eval_primary` из `eval_term`, а вот
`eval_term` из `eval_expr` вызывается уже не первый раз (первый вызов был в строке
75). Поднимемся вверх по стеку на два кадра и просмотрим локальные переменные:

    (gdb) up 2
    #2  0x00000000004009b3 in eval_expr (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe290) at calc.cpp:85
    85              rc = eval_term(in, &term);
    (gdb) info locals
    op = 43 '+'
    term = 1
    rc = E_OK
    cur_value = 1

По-видимому, мы считали из входного потока число "1", знак "+" и теперь должны
считать следующую лексему. Т.е. второй вызов `eval_primary` завершился с ошибкой
`E_EOF`. Проверим, так ли это. Выставим ещё одну точку останова на строку 35 и
перезапустим программу:

    (gdb) b 35
    Breakpoint 2 at 0x4007d7: file calc.cpp, line 35.
    (gdb) r < expr.txt 
    The program being debugged has been started already.
    Start it from the beginning? (y or n) y
    Starting program: /home/miyuki/projects/examples_gdb/calc < expr.txt

    Breakpoint 2, eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:35
    35          double cur_val = 0;
    (gdb) c
    Continuing.

    Breakpoint 2, eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:35
    35          double cur_val = 0;
    (gdb) c
    Continuing.

    Breakpoint 1, eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:44
    44              return E_EOF;

Верно. После двух срабатываний "Breakpoint 2" сработал "Breakpoint 1". Повторим
запуск и после второй остановки программы выполним её пошагово:

    Breakpoint 2, eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:35
    35          double cur_val = 0;
    (gdb) n
    36          while (char c = peek_not_eof(in)) {
    (gdb) n
    37              if (!(c >= '0' && c <= '9'))
    (gdb) p c
    $1 = 42 '*'
    (gdb) n
    43          if (!seen_digit)
    (gdb) n

    Breakpoint 1, eval_primary (in=0x7ffff75b54e0 <_IO_2_1_stdin_>, value=0x7fffffffe218) at calc.cpp:44
    44              return E_EOF;

Как видим, встретился символ "*" и программа вышла из цикла. Поскольку
переменная `seen_digit` равна своему изначальному значению `false`, условие `if`
выполняется и `eval_primary` возврщает `E_EOF`. Ошибка состоит в том, что нужно
возвращать `E_EOF` только если нам встретился конец файла, а в любом другом
случае - `E_INVAL`.

Исправить ошибку можно, к примеру, заменив блок `if` (строки 37-38) на
следующий:

```C++
if (!(c >= '0' && c <= '9')) {
    if (!seen_digit)
        return E_INVAL;
    break;
}
```

Исправленный вариант: [calc_fixed.cpp](examples_gdb/calc_fixed.cpp).

Скомпилируем исправленную программу и убедимся, что ошибка исчезла:

    $ g++ -o calc_fixed calc_fixed.cpp
    $ ./calc_fixed < expr.txt
    ERROR. Invalid expression
