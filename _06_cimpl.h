

#ifndef _INC_V_CIMPL
#define _INC_V_CIMPL

#include "_05_fileio.h"

extern hash CIMPL_tkn_include;
extern hash CIMPL_tkn_define;
extern hash CIMPL_tkn_defined;

extern hash CIMPL_tkn_undef;
extern hash CIMPL_tkn_ifdef;
extern hash CIMPL_tkn_ifndef;
extern hash CIMPL_tkn_elifdef;
extern hash CIMPL_tkn_elifndef;
extern hash CIMPL_tkn_endif;
extern hash CIMPL_tkn_error;
extern hash CIMPL_tkn_warning;
extern hash CIMPL_tkn_line;
extern hash CIMPL_tkn_pragma;

extern hash CIMPL_tkn_if;
extern hash CIMPL_tkn_elif;
extern hash CIMPL_tkn_else;

#define Enable_C_Token_Lists                        \
    \
    CIMPL_tkn_newline = hash_function("\n");                  \
    CIMPL_tkn_pre_processor_diretive = hash_function("#");    \
    CIMPL_tkn_include = hash_function("include");             \
    CIMPL_tkn_define = hash_function("define");               \
    CIMPL_tkn_defined = hash_function("defined");             \
    CIMPL_tkn_undef = hash_function("undef");                 \
    CIMPL_tkn_ifdef = hash_function("ifdef");                 \
    CIMPL_tkn_ifndef = hash_function("ifndef");               \
    CIMPL_tkn_elifdef = hash_function("elifdef");             \
    CIMPL_tkn_elifndef = hash_function("elifndef");           \
    CIMPL_tkn_endif = hash_function("endif");                 \
    CIMPL_tkn_error = hash_function("error");                 \
    CIMPL_tkn_warning = hash_function("warning");             \
    CIMPL_tkn_line = hash_function("line");                   \
    CIMPL_tkn_pragma = hash_function("pragma");               \
    CIMPL_tkn_if = hash_function("if");                       \
    CIMPL_tkn_elif = hash_function("elif");                   \
    CIMPL_tkn_else = hash_function("else");                   \
    \
    enable_token_list(DataTypes);                \
    enable_token_list(StorageClasses);           \
    enable_token_list(TypeQualifiers);           \
    enable_token_list(Controls);                 \
    enable_token_list(Structures);               \
    enable_token_list(Specifiers);               \
    enable_token_list(CompileTimeOperators);     \
    enable_token_list(Assertion);                \
    enable_token_list(Pre_Processor_Commands);   \
    enable_token_list(Pre_Processor_Operators)   \

#define CIMPL_Colorise_Red                            color_fore(240, 100, 100)
#define CIMPL_Colorise_High_Red       style_bold   ,  color_fore(240, 180, 150)
#define CIMPL_Colorise_Mild_Red                       color_fore(240, 180, 150)
#define CIMPL_Colorise_Err_Red        style_italics,  color_fore(250, 20 , 20 )
#define CIMPL_Colorise_Rest_Red       style_italics,  color_fore(240, 180, 150)
#define CIMPL_Colorise_Blue                           color_fore(150, 200, 240)
#define CIMPL_Colorise_High_Blue      style_bold   ,  color_fore(180, 220, 240)
#define CIMPL_Colorise_Rest_Blue      style_italics,  color_fore(150, 200, 240)
#define CIMPL_Colorise_Green                          color_fore(160, 240, 50 )
#define CIMPL_Colorise_High_Green     style_bold   ,  color_fore(200, 240, 100)
#define CIMPL_Colorise_Rest_Green     style_italics,  color_fore(160, 240, 50 )
#define CIMPL_Colorise_Purple                         color_fore(180, 150, 240)
#define CIMPL_Colorise_Yellow                         color_fore(200, 220, 150)

#define CIMPL_DeColorise color_fore_reset; style_bold_reset; style_italics_reset

extern uint16_t CIMPL_enclose_rgb[3];
extern int64_t CIMPL_scope_count;

#define CIMPL_Colorise_Brac                                             \
    if token_is(enclose, open) CIMPL_scope_count++;                     \
        color_fore(                                                     \
            CIMPL_enclose_rgb[0],                                       \
            CIMPL_enclose_rgb[1] + ( (CIMPL_scope_count - 1)*80 )%155,  \
            CIMPL_enclose_rgb[2] - ( (CIMPL_scope_count - 1)*50 )%200   \
        );                                                              \
    if token_is(enclose, close) CIMPL_scope_count--

    
extern hash CIMPL_in_quotes;
extern hash CIMPL_this_quote;

#define CIMPL_Colorise_Quot                                                     \
    if (!CIMPL_in_quotes) {CIMPL_in_quotes = hash_value; CIMPL_Colorise_Yellow;}   \
    else if token_is(CIMPL_in_quotes) CIMPL_in_quotes = 0

#define CIMPL_Colorise_C_Tokens                                                     \
    if token_is(quoting)                        {CIMPL_Colorise_Quot;}              \
    else if (CIMPL_in_quotes)                   CIMPL_Colorise_Yellow;              \
    else if (!CIMPL_in_quotes) {                                                    \
        CIMPL_DeColorise;                                                           \
        if token_in(DataTypes)                  CIMPL_Colorise_Blue;                \
        else if token_in(StorageClasses)        CIMPL_Colorise_Red;                 \
        else if token_in(TypeQualifiers)        CIMPL_Colorise_Rest_Red;            \
        else if token_in(Controls)              CIMPL_Colorise_Red;                 \
        else if token_in(Structures)            CIMPL_Colorise_Blue;                \
        else if token_in(Specifiers)            CIMPL_Colorise_Rest_Red;            \
        else if token_in(CompileTimeOperators)  CIMPL_Colorise_Rest_Red;            \
        else if token_in(Assertion)             CIMPL_Colorise_High_Red;            \
        else if token_in(Functions)             CIMPL_Colorise_Green;               \
        else if token_in(Macros)                CIMPL_Colorise_Green;               \
        else if token_is(numeric)               CIMPL_Colorise_Purple;              \
        else if token_is(operate)               CIMPL_Colorise_Red;                 \
        else if token_is(assign)                CIMPL_Colorise_Red;                 \
        else if token_is(enclose)               {CIMPL_Colorise_Brac;}              \
    }

extern hash CIMPL_tkn_newline;
extern hash CIMPL_tkn_pre_processor_diretive;
extern int pre_processor_state;
extern hash pre_processor_command;

#define Colorise_C_Tokens                                           \
    if token_is(CIMPL_tkn_newline) {                                \
        pre_processor_state = 1;                                    \
        pre_processor_command = 0;                                  \
    }                                                               \
    else if (pre_processor_state) switch (pre_processor_state) {    \
        case 1:                                                     \
            if token_is(CIMPL_tkn_pre_processor_diretive) {         \
                pre_processor_state = 2;                            \
                CIMPL_Colorise_Mild_Red;                            \
            }                                                       \
            else if token_is_not(disjoin, space) {                  \
                pre_processor_state = 0;                            \
                CIMPL_Colorise_C_Tokens;                            \
            }                                                       \
            break;                                                  \
        default:                                                    \
            CIMPL_preprocessinng();                                 \
            break;                                                  \
    }                                                               \
    else CIMPL_Colorise_C_Tokens

#define Handle_Pre_Processing void CIMPL_preprocessinng (void) 

Handle_Pre_Processing;

#define Scan_Identifiers_3  \
        For idvalid         \
            Join idvalid    \
            Join numeric    \
            Vary All

#define Scan_Spaces_2           \
        For disjoin space       \
            Join disjoin space  \
            Vary All

#define Detect_Number_System_4  \
        For '0' Char            \
            Join '0' Char       \
            Join 'b' Char       \
            Join 'x' Char       \
            Vary All

#define Scan_Digits_3           \
        For numeric             \
            Join numeric        \
            Join '.' Char       \
            Vary All

#define Scan_Numbers_2_4        \
        Detect_Number_System_4; \
        Scan_Digits_3

#define Else_Vary_1     \
        For All         \
            Vary All

#define Else_Join_1     \
        For All         \
            Join All

#define Cimpl_simple_C_reader(_VLU_name) VLU {  \
    Scan_Identifiers_3;                         \
    Scan_Spaces_2;                              \
    Scan_Numbers_2_4;                           \
    Else_Vary_1;                                \
    Make(_VLU_name, 8, 8);                      \
}

#endif