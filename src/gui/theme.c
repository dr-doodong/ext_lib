#include "ext_theme.h"

u32 sIndex;
hsla_t sDarkTheme[THEME_MAX] = { 0 };
hsla_t sLightTheme[THEME_MAX] = { 0 };
hsla_t sSpecialTheme[THEME_MAX] = { 0 };
hsla_t* sTheme[] = {
    sDarkTheme,
    sLightTheme,
    sSpecialTheme,
};

NVGcolor Theme_GetColor(ThemeColor pal, s32 alpha, f32 mult) {
    hsla_t col = sTheme[sIndex][pal];
    
    if (sIndex == 1)
        mult = 1 / mult;
    
    return nvgHSLA(col.h, col.s * mult, col.l * mult, clamp_max(alpha, 255));
}

void Theme_SmoothStepToCol(NVGcolor* src, NVGcolor target, f32 a, f32 b, f32 c) {
    Math_DelSmoothStepToF(&src->r, target.r, a, b, c);
    Math_DelSmoothStepToF(&src->g, target.g, a, b, c);
    Math_DelSmoothStepToF(&src->b, target.b, a, b, c);
    Math_DelSmoothStepToF(&src->a, target.a, a, b, c);
}

void Theme_Init(u32 themeId) {
    sIndex = themeId;
    {
        sDarkTheme[THEME_BASE] =          (hsla_t) { 000.0 / 360, 0.0, 0.15 };
        sDarkTheme[THEME_SPLIT] =          (hsla_t) { 000.0 / 360, 0.0, 0.11 };
        sDarkTheme[THEME_SHADOW] =        (hsla_t) { 000.0 / 360, 0.0, 0.07 };
        sDarkTheme[THEME_HIGHLIGHT] =     (hsla_t) { 000.0 / 360, 0.0, 0.80 };
        sDarkTheme[THEME_ELEMENT_LIGHT] = (hsla_t) { 000.0 / 360, 0.0, 0.60 };
        sDarkTheme[THEME_ELEMENT_BASE] =  (hsla_t) { 000.0 / 360, 0.0, 0.16 };
        sDarkTheme[THEME_ELEMENT_DARK] =  (hsla_t) { 000.0 / 360, 0.0, 0.10 };
        sDarkTheme[THEME_PRIM] =          (hsla_t) { 220.0 / 360, 0.7, 0.60 };
        sDarkTheme[THEME_DELETE] =          (hsla_t) { 0.0 / 360, 0.7, 0.60 };
        sDarkTheme[THEME_TEXT] =          (hsla_t) { 000.0 / 360, 0.0, 0.90 };
    }
    {
        sLightTheme[THEME_BASE] =          (hsla_t) { 000.0 / 360, 0.0, 1.0f - 0.15 };
        sLightTheme[THEME_SPLIT] =          (hsla_t) { 000.0 / 360, 0.0, 1.0f - 0.11 };
        sLightTheme[THEME_SHADOW] =        (hsla_t) { 000.0 / 360, 0.0, 1.0f - 0.07 };
        sLightTheme[THEME_HIGHLIGHT] =     (hsla_t) { 000.0 / 360, 0.0, 1.0f - 0.80 };
        sLightTheme[THEME_ELEMENT_LIGHT] = (hsla_t) { 000.0 / 360, 0.0, 1.0f - 0.60 };
        sLightTheme[THEME_ELEMENT_BASE] =  (hsla_t) { 000.0 / 360, 0.0, 1.0f - 0.16 };
        sLightTheme[THEME_ELEMENT_DARK] =  (hsla_t) { 000.0 / 360, 0.0, 1.0f - 0.10 };
        sLightTheme[THEME_PRIM] =          (hsla_t) { 10.0 / 360, 0.7, 1.0f - 0.60 };
        sLightTheme[THEME_DELETE] =          (hsla_t) { 0.0 / 360, 0.7, 0.60 };
        sLightTheme[THEME_TEXT] =          (hsla_t) { 000.0 / 360, 0.0, 1.0f - 0.90 };
    }
    {
        sSpecialTheme[THEME_BASE] =          (hsla_t) { 220.0 / 360, 0.1, 0.17f };
        sSpecialTheme[THEME_SPLIT] =          (hsla_t) { 000.0 / 360, 0.0, 1.0f - 0.15 };
        sSpecialTheme[THEME_SHADOW] =        (hsla_t) { 220.0 / 360, 0.2, 0.25f };
        sSpecialTheme[THEME_HIGHLIGHT] =     (hsla_t) { 070.0 / 360, 0.5, 0.75f };
        sSpecialTheme[THEME_ELEMENT_LIGHT] = (hsla_t) { 070.0 / 360, 0.5, 0.75f };
        sSpecialTheme[THEME_ELEMENT_BASE] =  (hsla_t) { 220.0 / 360, 0.1, 0.12f };
        sSpecialTheme[THEME_ELEMENT_DARK] =  (hsla_t) { 000.0 / 360, 0.0, 0.10f };
        sSpecialTheme[THEME_PRIM] =          (hsla_t) { 070.0 / 360, 0.3, 0.5f };
        sSpecialTheme[THEME_DELETE] =          (hsla_t) { 0.0 / 360, 0.7, 0.60 };
        sSpecialTheme[THEME_TEXT] =          (hsla_t) { 070.0 / 360, 0.1, 0.9f };
    }
}

NVGcolor Theme_Mix(f32 v, NVGcolor a, NVGcolor b) {
    return (NVGcolor) {
               .r = lerpf(v, a.r, b.r),
               .g = lerpf(v, a.g, b.g),
               .b = lerpf(v, a.b, b.b),
               .a = lerpf(v, a.a, b.a),
    };
}
