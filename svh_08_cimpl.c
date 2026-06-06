#include "svh_07_tokenlist.h"
#include v_implement

token_list(DataTypes, 9) {
    keyword(void);
    keyword(char);
    keyword(short);
    keyword(int);
    keyword(long);
    keyword(float);
    keyword(double);
    keyword(signed);
    keyword(unsigned);
}

token_list(StorageClasses, 6) {
    keyword(auto);
    keyword(register);
    keyword(static);
    keyword(extern);
    keyword(thread_local);
    keyword(_Thread_local);
}

token_list(TypeQualifiers, 4) {
    keyword(const);
    keyword(volatile);
    keyword(restrict);
    keyword(_Atomic);
}

token_list(Controls, 13) {
    keyword(if);
    keyword(else);
    keyword(switch);
    keyword(case);
    keyword(default);
    keyword(for);
    keyword(while);
    keyword(do);
    keyword(break);
    keyword(continue);
    keyword(return);
    keyword(goto);
    keyword(typedef);
}

token_list(Structures, 4) {
    keyword(struct);
    keyword(union);
    keyword(enum);
}

token_list(Specifiers, 1) {
    keyword(inline);
}

token_list(CompileTimeOperators, 6) {
    keyword(sizeof);
    keyword(alignof);
    keyword(_Generic);
    keyword(alignas);
    keyword(_Alignas);
    keyword(_Alignof);
}

token_list(Assertion, 2) {
    keyword(static_assert);
    keyword(_Static_assert);
}

token_list(Functions);

#define Enable_C_Token_Lists                \
    enable_token_set(DataTypes);            \
    enable_token_set(StorageClasses);       \
    enable_token_set(TypeQualifiers);       \
    enable_token_set(Controls);             \
    enable_token_set(Structures);           \
    enable_token_set(Specifiers);           \
    enable_token_set(CompileTimeOperators); \
    enable_token_set(Assertion)             \

#define Colorise_Red                            color_fore(240, 100, 100)
#define Colorise_High_Red       style_bold   ,  color_fore(240, 180, 150)
#define Colorise_Rest_Red       style_italics,  color_fore(240, 180, 150)
#define Colorise_Blue                           color_fore(150, 200, 240)
#define Colorise_High_Blue      style_bold   ,  color_fore(180, 220, 240)
#define Colorise_Rest_Blue      style_italics,  color_fore(150, 200, 240)
#define Colorise_Green                          color_fore(160, 240, 50 )
#define Colorise_High_Green     style_bold   ,  color_fore(200, 240, 100)
#define Colorise_Rest_Green     style_italics,  color_fore(160, 240, 50 )
#define Colorise_Purple                         color_fore(180, 150, 240)
#define Colorise_Yellow                         color_fore(200, 220, 150)

#define DeColorise color_fore_reset; style_bold_reset; style_italics_reset


uint16_t CTL_enclose_rgb[3] = {240, 100, 200};
int64_t CTL_scope_count = 0;

#define Colorise_Brac                                               \
    if token_is(enclose, open) CTL_scope_count++;                   \
        color_fore(                                                 \
            CTL_enclose_rgb[0],                                     \
            CTL_enclose_rgb[1] + ( (CTL_scope_count - 1)*80 )%155,  \
            CTL_enclose_rgb[2] - ( (CTL_scope_count - 1)*50 )%200   \
        );                                                          \
    if token_is(enclose, close) CTL_scope_count--

    
hash CTL_in_quotes = 0;

#define Colorise_Quot                                               \
    if (!CTL_in_quotes) CTL_in_quotes = get_fnv, Colorise_Yellow;   \
    else if token_is(CTL_in_quotes) CTL_in_quotes = 0

#define Colorise_C_Tokens                                                   \
    if token_is(quoting)                        {Colorise_Quot;}            \
    else if (!CTL_in_quotes) {                                              \
        DeColorise;                                                         \
        if token_in(DataTypes)                  Colorise_Blue;              \
        else if token_in(StorageClasses)        Colorise_Red;               \
        else if token_in(TypeQualifiers)        Colorise_Rest_Red;          \
        else if token_in(Controls)              Colorise_Red;               \
        else if token_in(Structures)            Colorise_Blue;              \
        else if token_in(Specifiers)            Colorise_Rest_Red;          \
        else if token_in(CompileTimeOperators)  Colorise_Rest_Red;          \
        else if token_in(Assertion)             Colorise_High_Red;          \
        else if token_in(Functions)             Colorise_Green;             \
        else if token_is(numeric)               Colorise_Purple;            \
        else if token_is(operate)               Colorise_Red;               \
        else if token_is(assign)                Colorise_Red;               \
        else if token_is(enclose)               {Colorise_Brac;}            \
    }


    

    

