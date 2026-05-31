#ifndef _INC_V_READWRITE
#define _INC_V_READWRITE
#include "svh_05_script.h"
#include <stdio.h>

#define RW_save_path_buffer(_path, _size, ...) char uniq(RW_path_) [_size] __VA_OPT__(; memcpy(uniq(RW_path_), _path, _size); const char * uniq(RW_temp_)) = _path

#define RW_save_path(_path, ...) RW_save_path_buffer(_path, 1024 __VA_OPT__(,) __VA_ARGS__)

typedef struct RW_file_struct {
    FILE * file;
    char * path;
} RW_file_struct;

#define RW_new_file_struct_ RW_file_struct uniq(RW_fs_);

#define RW_set_path_ uniq(RW_fs_).path = uniq(RW_path_);

extern FILE * RW_recent_open;

#define RW_set_file_(_mode) uniq(RW_fs_).file = (RW_recent_open = fopen( uniq(RW_path_), _mode )); RW_file_struct

#define Opened (RW_recent_open)

#define Open(_path, ...) RW_save_path(_path __VA_OPT__(,) __VA_ARGS__); RW_new_file_struct_ RW_set_path_
#define OpenPtr(_path) Open(_path, 0)

#define Mode ; uniq(RW_fs_).file = (RW_recent_open = fopen(uniq(RW_path_),

#define In(_iden) )); RW_file_struct _iden = uniq(RW_fs_)

#define RW_init_Writer RW_file_struct RW_writer_; fpos_t RW_writer_pos;

extern RW_file_struct RW_writer_;
extern fpos_t RW_writer_pos;

#define Writer RW_writer_  = uniq(RW_fs_); RW_writer_.file = (RW_recent_open = fopen(uniq(RW_path_), "w")); 

#define WriteChar(_char) fputc(_char, RW_writer_.file)

#define WriteLine(_line) fputs(_line, RW_writer_.file)

#define WriteFormat(_format, ...) fprintf(RW_writer_.file, _format __VA_OPT__(,) __VA_ARGS__)

#define UpdateWriterPos fgetpos(RW_writer_.file, &RW_writer_pos)

#define WriterPosType fpos_t

#define WriterPos RW_writer_pos

#define WriterGoTo(_pos) fsetpos(RW_writer_.file, &_pos)

#define ResetWriter rewind(RW_writer_.file)

#define CloseWriter fclose(RW_writer_.file)

#define RW_init_Reader RW_file_struct RW_reader_; fpos_t RW_reader_pos;

extern RW_file_struct RW_reader_;
extern fpos_t RW_reader_pos;

#define Reader RW_reader_ = uniq(RW_fs_); RW_reader_.file = (RW_recent_open = fopen(uniq(RW_path_), "r"));

#define ReadChar(_char) for (int _char = (fgetpos(RW_reader_.file, &RW_reader_pos), fgetc(RW_reader_.file)); _char != EOF; (fgetpos(RW_reader_.file, &RW_reader_pos), _char = fgetc(RW_reader_.file)))

#define ReadLine(_line, _size) for (char _line[_size]; (fgetpos(RW_reader_.file, &RW_reader_pos), fgets(_line, sizeof(_line), RW_reader_.file));)

#define UpdateReaderPos fgetpos(RW_reader_.file, &RW_reader_pos)

#define ReaderPosType fpos_t

#define ReaderPos RW_reader_pos

#define ReaderGoTo(_pos) fsetpos(RW_reader_.file, &_pos)

#define ResetReader rewind(RW_reader_.file)

#define Once(_code) {_code; break;}

#define CloseReader fclose(RW_reader_.file)

#endif