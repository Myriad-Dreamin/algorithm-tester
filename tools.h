
# ifndef TOOLS_H
# define TOOLS_H


# ifndef TYPEDEF_ARR_ELEMENT
# define TYPEDEF_ARR_ELEMENT
typedef int arr_element;
# endif


#include <iostream>
#include <stdexcept>

#include <bitset>
#include <ctime>
#include <windows.h>
#include <direct.h>		//for mkdir rmdir
#include <io.h>			//for access

#include <cassert>
#include <fstream>

// in Win32 access = _access, mkdir = _mkdir, rm = _rmdir
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#define RMDIR(a) _rmdir((a))

// ?*"<>/;
enum invalid_char_lo: unsigned long long
{
    double_quotes = 1ULL << 34,
    asterisk = 1ULL << 42,
    slash = 1ULL << 47,
    semicon = 1ULL << 59,
    less_than = 1ULL << 60,
    greater_than = 1ULL << 62,
    question_mark = 1ULL << 63
};
// \|
enum invalid_char_hi: unsigned long long
{
    backslash = 1ULL << (92 - 64),
    vertical_bar = 1ULL << (124 - 64),
};

// do not use -128 ~ -1 as key.
const std::bitset<128> invalid_char_maps_lo(
    invalid_char_lo::double_quotes ||
    invalid_char_lo::asterisk ||
    invalid_char_lo::slash ||
    invalid_char_lo::semicon ||
    invalid_char_lo::less_than ||
    invalid_char_lo::greater_than ||
    invalid_char_lo::question_mark
);
const std::bitset<128> invalid_char_maps_hi(
    invalid_char_hi::backslash ||
    invalid_char_hi::vertical_bar
);
const std::bitset<128> invalid_char_maps = (invalid_char_maps_hi << 64) | invalid_char_maps_lo;

/*
 * musec: 微秒
 * misec: 毫秒
 * sec: 秒
 */
enum time_unit {musec, misec, sec};

/*
 * 微秒计数器
 * 
 * @method: start(), stop(), restart(), clear()
 * @attribute: microsec(), millisec(), sec()
 * 
 * Windows Only
 */
class stop_watch
{
private:
    /* CPU时钟周期 */
    LARGE_INTEGER cpu_frequency;
    /* 计时起始时间 */
    LARGE_INTEGER watch_begin_time;
    /* 累积时间 */
    long long elapsed;

public:

    stop_watch () : elapsed(0)
    {
        QueryPerformanceFrequency(&cpu_frequency);
    }

    ~stop_watch () {}


    /* 开始计时 */
    void start ()
    {
        QueryPerformanceCounter(&watch_begin_time);
        return ;
    }

    /* 停止计时 */
    void stop ()
    {
        LARGE_INTEGER watch_end_time;
        QueryPerformanceCounter(&watch_end_time);
        elapsed += (watch_end_time.QuadPart - watch_begin_time.QuadPart) * 1000000.0 / cpu_frequency.QuadPart;
        return ;
    }

    /* 重启计时器 */
    void restart ()
    {
        elapsed = 0;
        start();
        return ;
    }

    /* 清零计时器 */
    void clear ()
    {
        elapsed = 0;
        return ;
    }

    // 以microsecond为单位返回elapsed
    double microsec ()
    {
        return static_cast<double>(elapsed);
    }

    // 以millisecond为单位返回elapsed
    double millisec ()
    {
        return elapsed / 1000.0;
    }
    // 以second为单位返回elapsed
    double sec ()
    {
        return elapsed / 1000000.0;
    }
};


/*
 * csv/txt文件类
 * 
 * @constructor: default, constructor(path, flag)
 * @method: open(path, flag), close(), newline(), block(out), output(out), get(in)
 * @attribute: opening()
 * 
 * inherited from fstream
 */
class FileHandle: public std::fstream
{
private:
    /* 打开方式 */
    std::ios_base::openmode handle_flag;
    /* opened: 是否打开, newlined: 是否为新行起始位置 (打开时不查询新行) */
    bool opened,newlined;
public:
    FileHandle ()
    {
        opened = false;
        newlined = false;
    }

    /*
     * @para file_path: 文件路径
     * @para open_flag: 打开方式
     */
    FileHandle (
        const std::string& file_path,
        const std::ios_base::openmode open_flag = std::ios::out | std::ios::app
    ): std::fstream(file_path, open_flag)
    {
        opened = std::fstream::is_open();
        newlined = false;
        handle_flag = open_flag;
    }

    /*
     * @para file_path: 文件路径
     * @para open_flag: 打开方式
     *
     * 关闭已打开的文件, 打开新文件
     */
    void open (
        const std::string& file_path,
        const std::ios_base::openmode open_flag = std::ios::out | std::ios::app
    )
    {
        if (opened) {
            std::fstream::close();
        }

        std::fstream::open(file_path, open_flag);

        opened = std::fstream::is_open();
        newlined = false;
        handle_flag = open_flag;
        return ;
    }

    /* 关闭已打开的文件 */
    void close ()
    {
        if (opened) {
            std::fstream::close();
        }

        newlined = false;
        opened = false;
        return ;
    }

    /* Handle状态 */
    bool opening ()
    {
        return opened;
    }

    /* 开启一段新行 */
    void newline ()
    {
        if (!opened) {
            throw std::domain_error("no file opened");
        }

        newlined = false;

        *this << std::endl;
        return ;
    }

    /* 写入一个csv块 */
    template<typename T>
    void block(const T &out_element)
    {
        if (!opened) {
            throw std::domain_error("no file opened");
        }

        if (newlined) {
            *this << ",";
        } else {
            newlined = true;
        }

        *this << out_element;
        return ;
    }

    /* 写入一个数据块 */
    template<typename T>
    void output(const T &out_element)
    {
        if (!opened) {
            throw std::domain_error("no file opened");
        }

        if (newlined) {
            *this << " ";
        } else {
            newlined = true;
        }

        *this << out_element;
        return ;
    }

    /* 读出一个数据块 */
    bool get(int &in_element)
    {
        if (!opened) {
            throw std::domain_error("no file opened");
        }

        if (this->eof()) {
            return false;
        }

        *this >> in_element;
        return true;
    }
};

/*
 * 数据集类
 * 
 * @constructor: constructor(path, dataset_name)
 * @method: set_path(path), set_name(name), build_set(), clear_set(), force_clear_set()
 * @method: dump_info(), load_info()
 * @method: new_testcase(), open_testcase(index), close_testcase()
 * @attribute: size()
 */
class DataSet
{
protected:
    /* 数据集路径 */
    std::string self_path;
    /* 数据集名称 */
    std::string self_name;
    /* 数据集文件前缀 */
    std::string self_file_prefix;

    /* 数据集文件Handle */
    FileHandle self_handle;

    /* 数据集Case个数 */
    int case_count;

    /*
     * @para strPath: 文件夹路径
     * @return success: 是否创建成功
     */
    bool make_dir (const std::string& strPath)
    {
        int i = 0;
        int nDirLen = strPath.length();
        if (nDirLen <= 0) {
                    return false;
        }
        char *pDirTemp = new char[nDirLen + 4];
        strPath.copy(pDirTemp, nDirLen + 1, 0);// +1 to copy '\0'
        pDirTemp[nDirLen] = '\0';

        if (pDirTemp[nDirLen - 1] != '\\' && pDirTemp[nDirLen - 1] != '/') {
            pDirTemp[nDirLen] = '/';
            pDirTemp[nDirLen + 1] = '\0';
            nDirLen++;
        }

        for (i = 0; i < nDirLen; i++) {
            if (pDirTemp[i] == '\\' || pDirTemp[i] == '/') {
                pDirTemp[i] = '\0';

                int statu;
                statu = ACCESS(pDirTemp, 0);
                if (statu != 0) {
                    statu = MKDIR(pDirTemp);
                    if (statu != 0) {
                        return false;
                    }
                }
                pDirTemp[i] = '/';
            }
        }
        delete[] pDirTemp;
        return true;
    }

    /*
     * @para strPath: 文件夹路径
     * @return success: 是否销毁成功
     */
    bool remove_dir(const std::string & path)
    {
        std::string strPath = path;
        struct _finddata_t fb;

        if (strPath.at(strPath.length() - 1) != '\\' || strPath.at(strPath.length() - 1) != '/') {
            strPath.append("\\");
        }
        std::string findPath = strPath + "*";
        intptr_t handle;
        handle = _findfirst(findPath.c_str(), &fb);

        if (handle != -1L) {
            std::string pathTemp;
            do {

                if (strcmp(fb.name, "..")!=0 && strcmp(fb.name, ".")!=0) {
                    pathTemp.clear();
                    pathTemp = strPath + std::string(fb.name);
                    if (fb.attrib == _A_SUBDIR) {
                        remove_dir(pathTemp.c_str());
                    } else {
                        remove(pathTemp.c_str());
                    }
                }
            } while (0 == _findnext(handle, &fb));
            _findclose(handle);
        }
        return !RMDIR(strPath.c_str());
    }

public:
    /*
     * @para fold_path: 文件夹路径
     * @para dataset_name: 数据集名
     */
    DataSet(const std::string &fold_path, const std::string &dataset_name)
    {
        set_path(fold_path);
        set_name(dataset_name);
        self_file_prefix = self_path + self_name;
        case_count = 0;
    }

    /*
     * @para fold_path: 文件夹路径
     * @return sucess: 是否设定成功
     */
    bool set_path(const std::string &fold_path)
    {
        if (fold_path.length() > 255) {
            return false;
        }
        for (auto character : fold_path) {
            if (character < 0) {
                return false;
            }
            if (character == '\\' || character == '/') {
                continue;
            }
            if (invalid_char_maps[character]) {
                return false;
            }
        }
        self_path = fold_path;
        if ((self_path.back() != '\\') && (self_path.back() != '/')) {
            if (self_path.length() == 255) {
                return false;
            }
            self_path.push_back('/');
        }
        self_file_prefix = self_path + self_name;
        return true;
    }

    /*
     * @para dataset_name: 数据集名
     * @return sucess: 是否设定成功
     */
    bool set_name(const std::string &dataset_name)
    {
        if (dataset_name.length() > 255) {
            return false;
        }
        for (auto character : dataset_name) {
            if (character < 0) {
                return false;
            }
            if (invalid_char_maps[character]) {
                return false;
            }
        }
        self_name = dataset_name;
        self_file_prefix = self_path + self_name;
        return true;
    }

    /*
     * @return sucess: 是否创建成功
     *
     * 创建数据集文件夹
     */
    bool build_set()
    {
        return make_dir(self_path);
    }

    /*
     * @return sucess: 是否清除成功
     *
     * 递归清除文件夹
     */
    bool clear_set()
    {
        std::cout << "warning! this operation will remove folds and files recursively" << std::endl;

        std::string get_answer;

        /* 必须确认清楚是否清除文件夹 */
        reanswer:
        std::cout << "continue?(Y/n)" << std::endl;
        std::cin >> get_answer;
        if (get_answer.length() != 1) {
            std::cout << "output Yy or Nn" << std::endl;
            goto reanswer;
        }

        switch(get_answer[0]) {
            case 'Y':case 'y': {
                case_count=0;
                return remove_dir(self_path);
            }
            case 'N':case 'n': {
                return false;
            }
            /* 输入不合法 */
            default : {
                std::cout << "output Yy or Nn" << std::endl;
                goto reanswer;
            }
        }
    }

    /*
     * @return sucess: 是否清除成功
     *
     * 强制清除文件夹
     */
    bool force_clear_set()
    {
        case_count=0;
        return remove_dir(self_path);
    }

    /* 新创建一个Case文件 */
    void new_testcase()
    {
        self_handle.open(
            self_file_prefix + std::to_string(++case_count) + ".txt",
            std::ios::ate | std::ios::out
        );

        if (!self_handle.opening()) {
            throw std::domain_error(
                "can't create " +
                self_file_prefix +
                std::to_string(++case_count) +
                ".txt"
            );
        }
    }

    /* 打开一个Case文件 */
    void open_testcase(const int idx)
    {
        self_handle.open(self_file_prefix + std::to_string(idx) + ".txt", std::ios::ate | std::ios::out);

        if (!self_handle.opening()) {
            throw std::domain_error("can't open " + self_file_prefix + std::to_string(idx) + ".txt");
        }
    }

    /* 关闭一个Case文件 */
    void close_testcase()
    {
        self_handle.close();
    }

    /* Case的个数 */
    int size()
    {
        return case_count;
    }

    /* 保存数据集设定 */
    virtual void dump_info()
    {
        self_handle.open(self_file_prefix + "_info.txt", std::ios::ate | std::ios::out);
        if (!self_handle.opening()) {
            throw std::domain_error(self_file_prefix + "_info.txt is not opening");
        }

        self_handle << case_count << std::endl;
        self_handle.close();
    }

    /* 读取数据集设定 */
    virtual void load_info()
    {
        self_handle.open(self_file_prefix + "_info.txt", std::ios::in);
        if (!self_handle.opening()) {
            throw std::domain_error(self_file_prefix + "_info.txt is not opening");
        }

        self_handle >> case_count;
        self_handle.close();
    }

};

/*
 * Integer数据集
 * 
 * @method: dump_info(), load_info(), batch_ints(), load_testcase_ints()
 * @attribute: possible_space_size()
 * 
 * 仅仅适合本次排序实验的子类
 */
class IntSet: public DataSet
{
protected:

    int max_int_count;
public:

    IntSet(const std::string &fold_path, const std::string &dataset_name) :
        DataSet(fold_path, dataset_name)
    {
        max_int_count = 0;
    }

    /* 保存Set信息 */
    virtual void dump_info()
    {
        self_handle.open(self_file_prefix + "_info.txt", std::ios::ate | std::ios::out);
        if (!self_handle.opening()) {
            throw std::domain_error(self_file_prefix + "_info.txt is not opening");
        }
        self_handle << case_count << std::endl;
        self_handle << max_int_count << std::endl;
        self_handle.close();
    }

    /* 读取Set信息 */
    virtual void load_info()
    {
        self_handle.open(self_file_prefix + "_info.txt", std::ios::in);
        if (!self_handle.opening()) {
            throw std::domain_error(self_file_prefix + "_info.txt is not opening");
        }
        self_handle >> case_count;
        self_handle >> max_int_count;
        self_handle.close();
    }

    /* 打包一组int到testcase */
    void batch_ints(const int batch_space[],const int len)
    {
        for (int i = 0; i < len; i++) {
            self_handle.output(batch_space[i]);
        }
        max_int_count = std::max(max_int_count, len);
    }

    /* 读取一个testcase到integer数组 */
    void load_testcase_ints(int *dump_space, int idx)
    {
        self_handle.open(self_file_prefix + std::to_string(idx) + ".txt", std::ios::in);
        if (!self_handle.opening()) {
            throw std::domain_error(self_file_prefix + std::to_string(idx) + ".txt is not opening");
        }
        while(self_handle.get(*dump_space))dump_space++;
        self_handle.close();
    }

    int possible_space_size()
    {
        return max_int_count;
    }
};

/* 打印数组 */
void print_arr(arr_element const load_arr[], const int len)
{
    # ifndef DONOTPRINT
    for (int i = 0; i < len; i++){
        std::cout << load_arr[i] << " ";
    }
    std::cout << std::endl;
    # endif
    return ;
}

/* 断言两个数组在[0, len)上等值 */
void assert_equal(arr_element const left_arr[], arr_element const right_arr[], const int len)
{
    for (int i = 0; i < len; i++)
    {
        assert(left_arr[i] == right_arr[i]);
    }
    return ;
}

/* 一个比较快的伪随机实现(只适合不强调随机性的场合) */
unsigned int rand_uint()
{
    static unsigned int seed = time(nullptr);
    return seed = ((seed * 23333) ^ 19991213);
}

# endif // TOOLS_H