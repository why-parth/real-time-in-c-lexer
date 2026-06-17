#define precompilation "v_tbi_compilation.txt"

#if !defined(__vfilename)
#define __vfilename(path) ( strrchr(path, '/') ? strrchr(path, '/') + 1 : strrchr(path, '\\') ? strrchr(path, '\\') + 1 : path )
#endif

#if defined(_WIN32) && !defined(__vshell_format_copy)                                               
#define __vshell_format_copy "copy %s \"tr-%s.c\" >nul"     
#else                                                               
#define __vshell_format_copy "cp %s \"tr-%s.c\""                
#endif      

#ifdef compilemain
#define precompilerfunction __v_other_main
#else
#define precompilerfunction main
#endif 

#ifdef compilemain
#define precompiler void __v__unused(void) {
#else
#define precompiler int precompilerfunction (int argc, char * argv[]) {                 \
    char _file_[256], _cmd_[256];                                                       \
    strcpy(_file_, __vfilename(__FILE__));                                              \
    \
    int i = 0;                                                                          \
    for (; _file_[i] != '.'; i++);                                                      \
    _file_[i] = '\0';                                                                   \
    \
    sprintf(_cmd_, __vshell_format_copy, __FILE__, _file_);                             \
    system(_cmd_);                                                                      \
    \
    char pathin[1024];                                                                  \
    char pathout[1024];                                                                 \
    \
    snprintf(pathout,  sizeof(pathout),  "./%s.c", _file_);                             \
    snprintf(pathin, sizeof(pathin), "./tr-%s.c", _file_);                              \
    \
    FILE * fileout = fopen(pathout, "r");                                               \
    FILE * filein = fopen(pathin, "w");                                                 \
    \
    if (1)
#endif

#define _path_buffer_size 1024
// #define readfile(_path) char _path_##_as[_path_buffer_size]; snprintf(_path_##_as, sizeof(_path_##_as),  "./%s.c", _file_)
// #define readfile(_path) char _path_##_as[_path_buffer_size]; snprintf(_path_##_as, sizeof(_path_##_as),  "./%s.c", _file_)

#define compile                                                                         \
    sprintf(_cmd_, "gcc \"tr-%s.c\" -Dcompilemain -o %s", _file_, _file_);              \
    system(_cmd_);                                                                      \
    \
    sprintf(_cmd_, ".\\%s", _file_);                                                    \
    system(_cmd_);

#define compile_custom(_file_source, file_name)                                         \
    sprintf(_cmd_, "gcc \"tr-%s.c\" -Dcompilemain -o %s", _file_, _file_);              \
    system(_cmd_);                                                                      \
    \
    sprintf(_cmd_, ".\\%s", _file_);                                                    \
    system(_cmd_);   

#define runmain __v_other_main(argc, argv)

int main (int argc, char * argv[]);

int __v_other_main (int argc, char * argv[]);
