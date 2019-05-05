
# ifndef TESTER
# define TESTER


# ifndef TYPEDEF_ARR_ELEMENT
# define TYPEDEF_ARR_ELEMENT
typedef int arr_element;
# endif


#include "tools.h"
#include <iostream>
#include <algorithm>

/*
 * 测试框架类
 * 
 * @constructor: constructor(space, size, [option]path)
 * @method: prepare(arr, len), run(arr, len, func), virtual primitive()
 * @method: begin_clock(), end_clock(), settle_clock(), show(unit), used_time(unit)
 * @method: open(), close(), write_open(), write_close(), block(out), newline()
 */

typedef void (*TESTER_FUNCP)();

class Tester
{
protected:

    stop_watch self_watch;

    FileHandle self_file;
    bool write_allowed;

    void run (arr_element const input_arr[], const int len, TESTER_FUNCP test_func)
    {

        begin_clock();
        test_func();
        end_clock();

        # ifdef DEBUG
        print_arr(arr, len);
        assert_equal(test_space, std_space, len);
        # endif
    }
public:

    Tester ()
    {
        write_allowed = false;
    }

    Tester (const std::string &file_path)
    {
        open(file_path);
    }

    virtual ~Tester ()
    {
        close();
    }

    inline void begin_clock ()
    {
        self_watch.start();
    }

    inline void end_clock ()
    {
        self_watch.stop();
    }

    inline void settle_clock ()
    {
        self_watch.clear();
    }

    virtual void primitive (arr_element const input_arr[], const int len) = 0;

    void show (const time_unit t_unit=time_unit::musec)
    {
        using std::cout;
        using std::endl;
        switch(t_unit) {
            case time_unit::musec: {
                cout << "used " << self_watch.microsec() << " musec" << endl;
                break ;
            }
            case time_unit::misec: {
                cout << "used " << self_watch.millisec() << " misec" << endl;
                break ;
            }
            case time_unit::sec: {
                cout << "used " << self_watch.sec() << " sec" << endl;
                break ;
            }
            default: {
                throw std::invalid_argument(
                    "It is impossible reached? Did you input a wrong time_unit to show?"
                );
                break ;
            }
        }
    }

    double used_time (const time_unit t_unit=time_unit::musec)
    {
        switch(t_unit) {
            case time_unit::musec: {
                return self_watch.microsec();
                break ;
            }
            case time_unit::misec: {
                return self_watch.millisec();
                break ;
            }
            case time_unit::sec: {
                return self_watch.sec();
                break ;
            }
            default: {
                throw std::invalid_argument(
                    "It is impossible reached? Did you input a wrong time_unit to show?"
                );
                break ;
            }
        }
    }

    void open(const std::string &file_path)
    {
        self_file.open(file_path, std::ios::out | std::ios::app);
        write_allowed = true;
    }

    void close()
    {
        self_file.close();
        write_allowed = false;
    }

    void write_open()
    {
        write_allowed = true;
    }

    void write_close()
    {
        write_allowed = false;
    }

    void newline ()
    {
        self_file.newline();
    }

    template<typename T>
    void block(const T &out_element)
    {
        self_file.block(out_element);
    }
};

# endif