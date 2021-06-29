#ifndef __OPENGL_H__
#define __OPENGL_H__

#include "Memstar.h"
#include "Fear.h"

#define OPENGL_BOUND_TEXTURE 0x0072A660
#define OPENGL_SET_ALPHA_SOURCE 0x0072A6E0

#define OPENGL32_GLACCUM					0x0087710C
#define OPENGL32_GLALPHAFUNC				0x00877110
#define OPENGL32_GLARETEXTURESRESIDENT		0x00877114
#define OPENGL32_GLARRAYELEMENT				0x00877118
#define OPENGL32_GLBEGIN					0x0087711C
#define OPENGL32_GLBINDTEXTURE				0x00877120
#define OPENGL32_GLBITMAP					0x00877124
#define OPENGL32_GLBLENDFUNC				0x00877128
#define OPENGL32_GLCALLLIST					0x0087712C
#define OPENGL32_GLCALLLISTS				0x00877130
#define OPENGL32_GLCLEAR					0x00877134
#define OPENGL32_GLCLEARACCUM				0x00877138
#define OPENGL32_GLCLEARCOLOR				0x0087713C
#define OPENGL32_GLCLEARDEPTH				0x00877140
#define OPENGL32_GLCLEARINDEX				0x00877144
#define OPENGL32_GLCLEARSTENCIL				0x00877148
#define OPENGL32_GLCLIPPLANE				0x0087714C
#define OPENGL32_GLCOLOR3B					0x00877150
#define OPENGL32_GLCOLOR3BV					0x00877154
#define OPENGL32_GLCOLOR3D					0x00877158
#define OPENGL32_GLCOLOR3DV					0x0087715C
#define OPENGL32_GLCOLOR3F					0x00877160
#define OPENGL32_GLCOLOR3FV					0x00877164
#define OPENGL32_GLCOLOR3I					0x00877168
#define OPENGL32_GLCOLOR3IV					0x0087716C
#define OPENGL32_GLCOLOR3S					0x00877170
#define OPENGL32_GLCOLOR3SV					0x00877174
#define OPENGL32_GLCOLOR3UB					0x00877178
#define OPENGL32_GLCOLOR3UBV				0x0087717C
#define OPENGL32_GLCOLOR3UI					0x00877180
#define OPENGL32_GLCOLOR3UIV				0x00877184
#define OPENGL32_GLCOLOR3US					0x00877188
#define OPENGL32_GLCOLOR3USV				0x0087718C
#define OPENGL32_GLCOLOR4B					0x00877190
#define OPENGL32_GLCOLOR4BV					0x00877194
#define OPENGL32_GLCOLOR4D					0x00877198
#define OPENGL32_GLCOLOR4DV					0x0087719C
#define OPENGL32_GLCOLOR4F					0x008771A0
#define OPENGL32_GLCOLOR4FV					0x008771A4
#define OPENGL32_GLCOLOR4I					0x008771A8
#define OPENGL32_GLCOLOR4IV					0x008771AC
#define OPENGL32_GLCOLOR4S					0x008771B0
#define OPENGL32_GLCOLOR4SV					0x008771B4
#define OPENGL32_GLCOLOR4UB					0x008771B8
#define OPENGL32_GLCOLOR4UBV				0x008771BC
#define OPENGL32_GLCOLOR4UI					0x008771C0
#define OPENGL32_GLCOLOR4UIV				0x008771C4
#define OPENGL32_GLCOLOR4US					0x008771C8
#define OPENGL32_GLCOLOR4USV				0x008771CC
#define OPENGL32_GLCOLORMASK				0x008771D0
#define OPENGL32_GLCOLORMATERIAL			0x008771D4
#define OPENGL32_GLCOLORPOINTER				0x008771D8
#define OPENGL32_GLCOPYPIXELS				0x008771DC
#define OPENGL32_GLCOPYTEXIMAGE1D			0x008771E0
#define OPENGL32_GLCOPYTEXIMAGE2D			0x008771E4
#define OPENGL32_GLCOPYTEXSUBIMAGE1D		0x008771E8
#define OPENGL32_GLCOPYTEXSUBIMAGE2D		0x008771EC
#define OPENGL32_GLCULLFACE					0x008771F0
#define OPENGL32_GLDELETELISTS				0x008771F4
#define OPENGL32_GLDELETETEXTURES			0x008771F8
#define OPENGL32_GLDEPTHFUNC				0x008771FC
#define OPENGL32_GLDEPTHMASK				0x00877200
#define OPENGL32_GLDEPTHRANGE				0x00877204
#define OPENGL32_GLDISABLE					0x00877208
#define OPENGL32_GLDISABLECLIENTSTATE		0x0087720C
#define OPENGL32_GLDRAWARRAYS				0x00877210
#define OPENGL32_GLDRAWBUFFER				0x00877214
#define OPENGL32_GLDRAWELEMENTS				0x00877218
#define OPENGL32_GLDRAWPIXELS				0x0087721C
#define OPENGL32_GLEDGEFLAG					0x00877220
#define OPENGL32_GLEDGEFLAGPOINTER			0x00877224
#define OPENGL32_GLEDGEFLAGV				0x00877228
#define OPENGL32_GLENABLE					0x0087722C
#define OPENGL32_GLENABLECLIENTSTATE		0x00877230
#define OPENGL32_GLEND						0x00877234
#define OPENGL32_GLENDLIST					0x00877238
#define OPENGL32_GLEVALCOORD1D				0x0087723C
#define OPENGL32_GLEVALCOORD1DV				0x00877240
#define OPENGL32_GLEVALCOORD1F				0x00877244
#define OPENGL32_GLEVALCOORD1FV				0x00877248
#define OPENGL32_GLEVALCOORD2D				0x0087724C
#define OPENGL32_GLEVALCOORD2DV				0x00877250
#define OPENGL32_GLEVALCOORD2F				0x00877254
#define OPENGL32_GLEVALCOORD2FV				0x00877258
#define OPENGL32_GLEVALMESH1				0x0087725C
#define OPENGL32_GLEVALMESH2				0x00877260
#define OPENGL32_GLEVALPOINT1				0x00877264
#define OPENGL32_GLEVALPOINT2				0x00877268
#define OPENGL32_GLFEEDBACKBUFFER			0x0087726C
#define OPENGL32_GLFINISH					0x00877270
#define OPENGL32_GLFLUSH					0x00877274
#define OPENGL32_GLFOGF						0x00877278
#define OPENGL32_GLFOGFV					0x0087727C
#define OPENGL32_GLFOGI						0x00877280
#define OPENGL32_GLFOGIV					0x00877284
#define OPENGL32_GLFRONTFACE				0x00877288
#define OPENGL32_GLFRUSTUM					0x0087728C
#define OPENGL32_GLGENLISTS					0x00877290
#define OPENGL32_GLGENTEXTURES				0x00877294
#define OPENGL32_GLGETBOOLEANV				0x00877298
#define OPENGL32_GLGETCLIPPLANE				0x0087729C
#define OPENGL32_GLGETDOUBLEV				0x008772A0
#define OPENGL32_GLGETERROR					0x008772A4
#define OPENGL32_GLGETFLOATV				0x008772A8
#define OPENGL32_GLGETINTEGERV				0x008772AC
#define OPENGL32_GLGETLIGHTFV				0x008772B0
#define OPENGL32_GLGETLIGHTIV				0x008772B4
#define OPENGL32_GLGETMAPDV					0x008772B8
#define OPENGL32_GLGETMAPFV					0x008772BC
#define OPENGL32_GLGETMAPIV					0x008772C0
#define OPENGL32_GLGETMATERIALFV			0x008772C4
#define OPENGL32_GLGETMATERIALIV			0x008772C8
#define OPENGL32_GLGETPIXELMAPFV			0x008772CC
#define OPENGL32_GLGETPIXELMAPUIV			0x008772D0
#define OPENGL32_GLGETPIXELMAPUSV			0x008772D4
#define OPENGL32_GLGETPOINTERV				0x008772D8
#define OPENGL32_GLGETPOLYGONSTIPPLE		0x008772DC
#define OPENGL32_GLGETSTRING				0x008772E0
#define OPENGL32_GLGETTEXENVFV				0x008772E4
#define OPENGL32_GLGETTEXENVIV				0x008772E8
#define OPENGL32_GLGETTEXGENDV				0x008772EC
#define OPENGL32_GLGETTEXGENFV				0x008772F0
#define OPENGL32_GLGETTEXGENIV				0x008772F4
#define OPENGL32_GLGETTEXIMAGE				0x008772F8
#define OPENGL32_GLGETTEXLEVELPARAMETERFV	0x008772FC
#define OPENGL32_GLGETTEXLEVELPARAMETERIV	0x00877300
#define OPENGL32_GLGETTEXPARAMETERFV		0x00877304
#define OPENGL32_GLGETTEXPARAMETERIV		0x00877308
#define OPENGL32_GLHINT						0x0087730C
#define OPENGL32_GLINDEXMASK				0x00877310
#define OPENGL32_GLINDEXPOINTER				0x00877314
#define OPENGL32_GLINDEXD					0x00877318
#define OPENGL32_GLINDEXDV					0x0087731C
#define OPENGL32_GLINDEXF					0x00877320
#define OPENGL32_GLINDEXFV					0x00877324
#define OPENGL32_GLINDEXI					0x00877328
#define OPENGL32_GLINDEXIV					0x0087732C
#define OPENGL32_GLINDEXS					0x00877330
#define OPENGL32_GLINDEXSV					0x00877334
#define OPENGL32_GLINDEXUB					0x00877338
#define OPENGL32_GLINDEXUBV					0x0087733C
#define OPENGL32_GLINITNAMES				0x00877340
#define OPENGL32_GLINTERLEAVEDARRAYS		0x00877344
#define OPENGL32_GLISENABLED				0x00877348
#define OPENGL32_GLISLIST					0x0087734C
#define OPENGL32_GLISTEXTURE				0x00877350
#define OPENGL32_GLLIGHTMODELF				0x00877354
#define OPENGL32_GLLIGHTMODELFV				0x00877358
#define OPENGL32_GLLIGHTMODELI				0x0087735C
#define OPENGL32_GLLIGHTMODELIV				0x00877360
#define OPENGL32_GLLIGHTF					0x00877364
#define OPENGL32_GLLIGHTFV					0x00877368
#define OPENGL32_GLLIGHTI					0x0087736C
#define OPENGL32_GLLIGHTIV					0x00877370
#define OPENGL32_GLLINESTIPPLE				0x00877374
#define OPENGL32_GLLINEWIDTH				0x00877378
#define OPENGL32_GLLISTBASE					0x0087737C
#define OPENGL32_GLLOADIDENTITY				0x00877380
#define OPENGL32_GLLOADMATRIXD				0x00877384
#define OPENGL32_GLLOADMATRIXF				0x00877388
#define OPENGL32_GLLOADNAME					0x0087738C
#define OPENGL32_GLLOGICOP					0x00877390
#define OPENGL32_GLMAP1D					0x00877394
#define OPENGL32_GLMAP1F					0x00877398
#define OPENGL32_GLMAP2D					0x0087739C
#define OPENGL32_GLMAP2F					0x008773A0
#define OPENGL32_GLMAPGRID1D				0x008773A4
#define OPENGL32_GLMAPGRID1F				0x008773A8
#define OPENGL32_GLMAPGRID2D				0x008773AC
#define OPENGL32_GLMAPGRID2F				0x008773B0
#define OPENGL32_GLMATERIALF				0x008773B4
#define OPENGL32_GLMATERIALFV				0x008773B8
#define OPENGL32_GLMATERIALI				0x008773BC
#define OPENGL32_GLMATERIALIV				0x008773C0
#define OPENGL32_GLMATRIXMODE				0x008773C4
#define OPENGL32_GLMULTMATRIXD				0x008773C8
#define OPENGL32_GLMULTMATRIXF				0x008773CC
#define OPENGL32_GLNEWLIST					0x008773D0
#define OPENGL32_GLNORMAL3B					0x008773D4
#define OPENGL32_GLNORMAL3BV				0x008773D8
#define OPENGL32_GLNORMAL3D					0x008773DC
#define OPENGL32_GLNORMAL3DV				0x008773E0
#define OPENGL32_GLNORMAL3F					0x008773E4
#define OPENGL32_GLNORMAL3FV				0x008773E8
#define OPENGL32_GLNORMAL3I					0x008773EC
#define OPENGL32_GLNORMAL3IV				0x008773F0
#define OPENGL32_GLNORMAL3S					0x008773F4
#define OPENGL32_GLNORMAL3SV				0x008773F8
#define OPENGL32_GLNORMALPOINTER			0x008773FC
#define OPENGL32_GLORTHO					0x00877400
#define OPENGL32_GLPASSTHROUGH				0x00877404
#define OPENGL32_GLPIXELMAPFV				0x00877408
#define OPENGL32_GLPIXELMAPUIV				0x0087740C
#define OPENGL32_GLPIXELMAPUSV				0x00877410
#define OPENGL32_GLPIXELSTOREF				0x00877414
#define OPENGL32_GLPIXELSTOREI				0x00877418
#define OPENGL32_GLPIXELTRANSFERF			0x0087741C
#define OPENGL32_GLPIXELTRANSFERI			0x00877420
#define OPENGL32_GLPIXELZOOM				0x00877424
#define OPENGL32_GLPOINTSIZE				0x00877428
#define OPENGL32_GLPOLYGONMODE				0x0087742C
#define OPENGL32_GLPOLYGONOFFSET			0x00877430
#define OPENGL32_GLPOLYGONSTIPPLE			0x00877434
#define OPENGL32_GLPOPATTRIB				0x00877438
#define OPENGL32_GLPOPCLIENTATTRIB			0x0087743C
#define OPENGL32_GLPOPMATRIX				0x00877440
#define OPENGL32_GLPOPNAME					0x00877444
#define OPENGL32_GLPRIORITIZETEXTURES		0x00877448
#define OPENGL32_GLPUSHATTRIB				0x0087744C
#define OPENGL32_GLPUSHCLIENTATTRIB			0x00877450
#define OPENGL32_GLPUSHMATRIX				0x00877454
#define OPENGL32_GLPUSHNAME					0x00877458
#define OPENGL32_GLRASTERPOS2D				0x0087745C
#define OPENGL32_GLRASTERPOS2DV				0x00877460
#define OPENGL32_GLRASTERPOS2F				0x00877464
#define OPENGL32_GLRASTERPOS2FV				0x00877468
#define OPENGL32_GLRASTERPOS2I				0x0087746C
#define OPENGL32_GLRASTERPOS2IV				0x00877470
#define OPENGL32_GLRASTERPOS2S				0x00877474
#define OPENGL32_GLRASTERPOS2SV				0x00877478
#define OPENGL32_GLRASTERPOS3D				0x0087747C
#define OPENGL32_GLRASTERPOS3DV				0x00877480
#define OPENGL32_GLRASTERPOS3F				0x00877484
#define OPENGL32_GLRASTERPOS3FV				0x00877488
#define OPENGL32_GLRASTERPOS3I				0x0087748C
#define OPENGL32_GLRASTERPOS3IV				0x00877490
#define OPENGL32_GLRASTERPOS3S				0x00877494
#define OPENGL32_GLRASTERPOS3SV				0x00877498
#define OPENGL32_GLRASTERPOS4D				0x0087749C
#define OPENGL32_GLRASTERPOS4DV				0x008774A0
#define OPENGL32_GLRASTERPOS4F				0x008774A4
#define OPENGL32_GLRASTERPOS4FV				0x008774A8
#define OPENGL32_GLRASTERPOS4I				0x008774AC
#define OPENGL32_GLRASTERPOS4IV				0x008774B0
#define OPENGL32_GLRASTERPOS4S				0x008774B4
#define OPENGL32_GLRASTERPOS4SV				0x008774B8
#define OPENGL32_GLREADBUFFER				0x008774BC
#define OPENGL32_GLREADPIXELS				0x008774C0
#define OPENGL32_GLRECTD					0x008774C4
#define OPENGL32_GLRECTDV					0x008774C8
#define OPENGL32_GLRECTF					0x008774CC
#define OPENGL32_GLRECTFV					0x008774D0
#define OPENGL32_GLRECTI					0x008774D4
#define OPENGL32_GLRECTIV					0x008774D8
#define OPENGL32_GLRECTS					0x008774DC
#define OPENGL32_GLRECTSV					0x008774E0
#define OPENGL32_GLRENDERMODE				0x008774E4
#define OPENGL32_GLROTATED					0x008774E8
#define OPENGL32_GLROTATEF					0x008774EC
#define OPENGL32_GLSCALED					0x008774F0
#define OPENGL32_GLSCALEF					0x008774F4
#define OPENGL32_GLSCISSOR					0x008774F8
#define OPENGL32_GLSELECTBUFFER				0x008774FC
#define OPENGL32_GLSHADEMODEL				0x00877500
#define OPENGL32_GLSTENCILFUNC				0x00877504
#define OPENGL32_GLSTENCILMASK				0x00877508
#define OPENGL32_GLSTENCILOP				0x0087750C
#define OPENGL32_GLTEXCOORD1D				0x00877510
#define OPENGL32_GLTEXCOORD1DV				0x00877514
#define OPENGL32_GLTEXCOORD1F				0x00877518
#define OPENGL32_GLTEXCOORD1FV				0x0087751C
#define OPENGL32_GLTEXCOORD1I				0x00877520
#define OPENGL32_GLTEXCOORD1IV				0x00877524
#define OPENGL32_GLTEXCOORD1S				0x00877528
#define OPENGL32_GLTEXCOORD1SV				0x0087752C
#define OPENGL32_GLTEXCOORD2D				0x00877530
#define OPENGL32_GLTEXCOORD2DV				0x00877534
#define OPENGL32_GLTEXCOORD2F				0x00877538
#define OPENGL32_GLTEXCOORD2FV				0x0087753C
#define OPENGL32_GLTEXCOORD2I				0x00877540
#define OPENGL32_GLTEXCOORD2IV				0x00877544
#define OPENGL32_GLTEXCOORD2S				0x00877548
#define OPENGL32_GLTEXCOORD2SV				0x0087754C
#define OPENGL32_GLTEXCOORD3D				0x00877550
#define OPENGL32_GLTEXCOORD3DV				0x00877554
#define OPENGL32_GLTEXCOORD3F				0x00877558
#define OPENGL32_GLTEXCOORD3FV				0x0087755C
#define OPENGL32_GLTEXCOORD3I				0x00877560
#define OPENGL32_GLTEXCOORD3IV				0x00877564
#define OPENGL32_GLTEXCOORD3S				0x00877568
#define OPENGL32_GLTEXCOORD3SV				0x0087756C
#define OPENGL32_GLTEXCOORD4D				0x00877570
#define OPENGL32_GLTEXCOORD4DV				0x00877574
#define OPENGL32_GLTEXCOORD4F				0x00877578
#define OPENGL32_GLTEXCOORD4FV				0x0087757C
#define OPENGL32_GLTEXCOORD4I				0x00877580
#define OPENGL32_GLTEXCOORD4IV				0x00877584
#define OPENGL32_GLTEXCOORD4S				0x00877588
#define OPENGL32_GLTEXCOORD4SV				0x0087758C
#define OPENGL32_GLTEXCOORDPOINTER			0x00877590
#define OPENGL32_GLTEXENVF					0x00877594
#define OPENGL32_GLTEXENVFV					0x00877598
#define OPENGL32_GLTEXENVI					0x0087759C
#define OPENGL32_GLTEXENVIV					0x008775A0
#define OPENGL32_GLTEXGEND					0x008775A4
#define OPENGL32_GLTEXGENDV					0x008775A8
#define OPENGL32_GLTEXGENF					0x008775AC
#define OPENGL32_GLTEXGENFV					0x008775B0
#define OPENGL32_GLTEXGENI					0x008775B4
#define OPENGL32_GLTEXGENIV					0x008775B8
#define OPENGL32_GLTEXIMAGE1D				0x008775BC
#define OPENGL32_GLTEXIMAGE2D				0x008775C0
#define OPENGL32_GLTEXPARAMETERF			0x008775C4
#define OPENGL32_GLTEXPARAMETERFV			0x008775C8
#define OPENGL32_GLTEXPARAMETERI			0x008775CC
#define OPENGL32_GLTEXPARAMETERIV			0x008775D0
#define OPENGL32_GLTEXSUBIMAGE1D			0x008775D4
#define OPENGL32_GLTEXSUBIMAGE2D			0x008775D8
#define OPENGL32_GLTRANSLATED				0x008775DC
#define OPENGL32_GLTRANSLATEF				0x008775E0
#define OPENGL32_GLVERTEX2D					0x008775E4
#define OPENGL32_GLVERTEX2DV				0x008775E8
#define OPENGL32_GLVERTEX2F					0x008775EC
#define OPENGL32_GLVERTEX2FV				0x008775F0
#define OPENGL32_GLVERTEX2I					0x008775F4
#define OPENGL32_GLVERTEX2IV				0x008775F8
#define OPENGL32_GLVERTEX2S					0x008775FC
#define OPENGL32_GLVERTEX2SV				0x00877600
#define OPENGL32_GLVERTEX3D					0x00877604
#define OPENGL32_GLVERTEX3DV				0x00877608
#define OPENGL32_GLVERTEX3F					0x0087760C
#define OPENGL32_GLVERTEX3FV				0x00877610
#define OPENGL32_GLVERTEX3I					0x00877614
#define OPENGL32_GLVERTEX3IV				0x00877618
#define OPENGL32_GLVERTEX3S					0x0087761C
#define OPENGL32_GLVERTEX3SV				0x00877620
#define OPENGL32_GLVERTEX4D					0x00877624
#define OPENGL32_GLVERTEX4DV				0x00877628
#define OPENGL32_GLVERTEX4F					0x0087762C
#define OPENGL32_GLVERTEX4FV				0x00877630
#define OPENGL32_GLVERTEX4I					0x00877634
#define OPENGL32_GLVERTEX4IV				0x00877638
#define OPENGL32_GLVERTEX4S					0x0087763C
#define OPENGL32_GLVERTEX4SV				0x00877640
#define OPENGL32_GLVERTEXPOINTER			0x00877644
#define OPENGL32_GLVIEWPORT					0x00877648
#define OPENGL32_WGLCHOOSEPIXELFORMAT		0x0087764C
#define OPENGL32_WGLCOPYCONTEXT				0x00877650
#define OPENGL32_WGLCREATECONTEXT			0x00877654
#define OPENGL32_WGLCREATELAYERCONTEXT		0x00877658
#define OPENGL32_WGLDELETECONTEXT			0x0087765C
#define OPENGL32_WGLDESCRIBELAYERPLANE		0x00877660
#define OPENGL32_WGLDESCRIBEPIXELFORMAT		0x00877664
#define OPENGL32_WGLGETCURRENTCONTEXT		0x00877668
#define OPENGL32_WGLGETCURRENTDC			0x0087766C
#define OPENGL32_WGLGETDEFAULTPROCADDRESS	0x00877670
#define OPENGL32_WGLGETLAYERPALETTEENTRIES	0x00877674
#define OPENGL32_WGLGETPIXELFORMAT			0x00877678
#define OPENGL32_WGLGETPROCADDRESS			0x0087767C
#define OPENGL32_WGLMAKECURRENT				0x00877680
#define OPENGL32_WGLREALIZELAYERPALETTE		0x00877684
#define OPENGL32_WGLSETLAYERPALETTEENTRIES	0x00877688
#define OPENGL32_WGLSETPIXELFORMAT			0x0087768C
#define OPENGL32_WGLSHARELISTS				0x00877690
#define OPENGL32_WGLSWAPBUFFERS				0x00877694
#define OPENGL32_WGLSWAPLAYERBUFFERS		0x00877698
#define OPENGL32_WGLUSEFONTBITMAPSA			0x0087769C
#define OPENGL32_WGLUSEFONTBITMAPSW			0x008776A0
#define OPENGL32_WGLUSEFONTOUTLINESA		0x008776A4
#define OPENGL32_WGLUSEFONTOUTLINESW		0x008776A8


typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;


/* Version */
#define GL_VERSION_1_1                    1

/* AccumOp */
#define GL_ACCUM                          0x0100
#define GL_LOAD                           0x0101
#define GL_RETURN                         0x0102
#define GL_MULT                           0x0103
#define GL_ADD                            0x0104

/* AlphaFunction */
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NOTEQUAL                       0x0205
#define GL_GEQUAL                         0x0206
#define GL_ALWAYS                         0x0207

/* AttribMask */
#define GL_CURRENT_BIT                    0x00000001
#define GL_POINT_BIT                      0x00000002
#define GL_LINE_BIT                       0x00000004
#define GL_POLYGON_BIT                    0x00000008
#define GL_POLYGON_STIPPLE_BIT            0x00000010
#define GL_PIXEL_MODE_BIT                 0x00000020
#define GL_LIGHTING_BIT                   0x00000040
#define GL_FOG_BIT                        0x00000080
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_ACCUM_BUFFER_BIT               0x00000200
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_VIEWPORT_BIT                   0x00000800
#define GL_TRANSFORM_BIT                  0x00001000
#define GL_ENABLE_BIT                     0x00002000
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_HINT_BIT                       0x00008000
#define GL_EVAL_BIT                       0x00010000
#define GL_LIST_BIT                       0x00020000
#define GL_TEXTURE_BIT                    0x00040000
#define GL_SCISSOR_BIT                    0x00080000
#define GL_ALL_ATTRIB_BITS                0x000fffff

/* BeginMode */
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_QUADS                          0x0007
#define GL_QUAD_STRIP                     0x0008
#define GL_POLYGON                        0x0009

/* BlendingFactorDest */
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305

/* BlendingFactorSrc */
/*      GL_ZERO */
/*      GL_ONE */
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
/*      GL_SRC_ALPHA */
/*      GL_ONE_MINUS_SRC_ALPHA */
/*      GL_DST_ALPHA */
/*      GL_ONE_MINUS_DST_ALPHA */

/* Boolean */
#define GL_TRUE                           1
#define GL_FALSE                          0

/* ClearBufferMask */
/*      GL_COLOR_BUFFER_BIT */
/*      GL_ACCUM_BUFFER_BIT */
/*      GL_STENCIL_BUFFER_BIT */
/*      GL_DEPTH_BUFFER_BIT */

/* ClientArrayType */
/*      GL_VERTEX_ARRAY */
/*      GL_NORMAL_ARRAY */
/*      GL_COLOR_ARRAY */
/*      GL_INDEX_ARRAY */
/*      GL_TEXTURE_COORD_ARRAY */
/*      GL_EDGE_FLAG_ARRAY */

/* ClipPlaneName */
#define GL_CLIP_PLANE0                    0x3000
#define GL_CLIP_PLANE1                    0x3001
#define GL_CLIP_PLANE2                    0x3002
#define GL_CLIP_PLANE3                    0x3003
#define GL_CLIP_PLANE4                    0x3004
#define GL_CLIP_PLANE5                    0x3005

/* ColorMaterialFace */
/*      GL_FRONT */
/*      GL_BACK */
/*      GL_FRONT_AND_BACK */

/* ColorMaterialParameter */
/*      GL_AMBIENT */
/*      GL_DIFFUSE */
/*      GL_SPECULAR */
/*      GL_EMISSION */
/*      GL_AMBIENT_AND_DIFFUSE */

/* ColorPointerType */
/*      GL_BYTE */
/*      GL_UNSIGNED_BYTE */
/*      GL_SHORT */
/*      GL_UNSIGNED_SHORT */
/*      GL_INT */
/*      GL_UNSIGNED_INT */
/*      GL_FLOAT */
/*      GL_DOUBLE */

/* CullFaceMode */
/*      GL_FRONT */
/*      GL_BACK */
/*      GL_FRONT_AND_BACK */

/* DataType */
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_2_BYTES                        0x1407
#define GL_3_BYTES                        0x1408
#define GL_4_BYTES                        0x1409
#define GL_DOUBLE                         0x140A

/* OpenGL 1.2 Pixel Crap */
#define GL_BGR                            0x80E0
#define GL_BGRA                           0x80E1
#define GL_UNSIGNED_BYTE_3_3_2            0x8032
#define GL_UNSIGNED_BYTE_2_3_3_REV        0x8362
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV       0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_4_4_4_4_REV     0x8365
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_SHORT_1_5_5_5_REV     0x8366
#define GL_UNSIGNED_INT_8_8_8_8           0x8035
#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367
#define GL_UNSIGNED_INT_10_10_10_2        0x8036
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368


/* DepthFunction */
/*      GL_NEVER */
/*      GL_LESS */
/*      GL_EQUAL */
/*      GL_LEQUAL */
/*      GL_GREATER */
/*      GL_NOTEQUAL */
/*      GL_GEQUAL */
/*      GL_ALWAYS */

/* DrawBufferMode */
#define GL_NONE                           0
#define GL_FRONT_LEFT                     0x0400
#define GL_FRONT_RIGHT                    0x0401
#define GL_BACK_LEFT                      0x0402
#define GL_BACK_RIGHT                     0x0403
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_LEFT                           0x0406
#define GL_RIGHT                          0x0407
#define GL_FRONT_AND_BACK                 0x0408
#define GL_AUX0                           0x0409
#define GL_AUX1                           0x040A
#define GL_AUX2                           0x040B
#define GL_AUX3                           0x040C

/* Enable */
/*      GL_FOG */
/*      GL_LIGHTING */
/*      GL_TEXTURE_1D */
/*      GL_TEXTURE_2D */
/*      GL_LINE_STIPPLE */
/*      GL_POLYGON_STIPPLE */
/*      GL_CULL_FACE */
/*      GL_ALPHA_TEST */
/*      GL_BLEND */
/*      GL_INDEX_LOGIC_OP */
/*      GL_COLOR_LOGIC_OP */
/*      GL_DITHER */
/*      GL_STENCIL_TEST */
/*      GL_DEPTH_TEST */
/*      GL_CLIP_PLANE0 */
/*      GL_CLIP_PLANE1 */
/*      GL_CLIP_PLANE2 */
/*      GL_CLIP_PLANE3 */
/*      GL_CLIP_PLANE4 */
/*      GL_CLIP_PLANE5 */
/*      GL_LIGHT0 */
/*      GL_LIGHT1 */
/*      GL_LIGHT2 */
/*      GL_LIGHT3 */
/*      GL_LIGHT4 */
/*      GL_LIGHT5 */
/*      GL_LIGHT6 */
/*      GL_LIGHT7 */
/*      GL_TEXTURE_GEN_S */
/*      GL_TEXTURE_GEN_T */
/*      GL_TEXTURE_GEN_R */
/*      GL_TEXTURE_GEN_Q */
/*      GL_MAP1_VERTEX_3 */
/*      GL_MAP1_VERTEX_4 */
/*      GL_MAP1_COLOR_4 */
/*      GL_MAP1_INDEX */
/*      GL_MAP1_NORMAL */
/*      GL_MAP1_TEXTURE_COORD_1 */
/*      GL_MAP1_TEXTURE_COORD_2 */
/*      GL_MAP1_TEXTURE_COORD_3 */
/*      GL_MAP1_TEXTURE_COORD_4 */
/*      GL_MAP2_VERTEX_3 */
/*      GL_MAP2_VERTEX_4 */
/*      GL_MAP2_COLOR_4 */
/*      GL_MAP2_INDEX */
/*      GL_MAP2_NORMAL */
/*      GL_MAP2_TEXTURE_COORD_1 */
/*      GL_MAP2_TEXTURE_COORD_2 */
/*      GL_MAP2_TEXTURE_COORD_3 */
/*      GL_MAP2_TEXTURE_COORD_4 */
/*      GL_POINT_SMOOTH */
/*      GL_LINE_SMOOTH */
/*      GL_POLYGON_SMOOTH */
/*      GL_SCISSOR_TEST */
/*      GL_COLOR_MATERIAL */
/*      GL_NORMALIZE */
/*      GL_AUTO_NORMAL */
/*      GL_VERTEX_ARRAY */
/*      GL_NORMAL_ARRAY */
/*      GL_COLOR_ARRAY */
/*      GL_INDEX_ARRAY */
/*      GL_TEXTURE_COORD_ARRAY */
/*      GL_EDGE_FLAG_ARRAY */
/*      GL_POLYGON_OFFSET_POINT */
/*      GL_POLYGON_OFFSET_LINE */
/*      GL_POLYGON_OFFSET_FILL */

/* ErrorCode */
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_STACK_OVERFLOW                 0x0503
#define GL_STACK_UNDERFLOW                0x0504
#define GL_OUT_OF_MEMORY                  0x0505

/* FeedBackMode */
#define GL_2D                             0x0600
#define GL_3D                             0x0601
#define GL_3D_COLOR                       0x0602
#define GL_3D_COLOR_TEXTURE               0x0603
#define GL_4D_COLOR_TEXTURE               0x0604

/* FeedBackToken */
#define GL_PASS_THROUGH_TOKEN             0x0700
#define GL_POINT_TOKEN                    0x0701
#define GL_LINE_TOKEN                     0x0702
#define GL_POLYGON_TOKEN                  0x0703
#define GL_BITMAP_TOKEN                   0x0704
#define GL_DRAW_PIXEL_TOKEN               0x0705
#define GL_COPY_PIXEL_TOKEN               0x0706
#define GL_LINE_RESET_TOKEN               0x0707

/* FogMode */
/*      GL_LINEAR */
#define GL_EXP                            0x0800
#define GL_EXP2                           0x0801


/* FogParameter */
/*      GL_FOG_COLOR */
/*      GL_FOG_DENSITY */
/*      GL_FOG_END */
/*      GL_FOG_INDEX */
/*      GL_FOG_MODE */
/*      GL_FOG_START */

/* FrontFaceDirection */
#define GL_CW                             0x0900
#define GL_CCW                            0x0901

/* GetMapTarget */
#define GL_COEFF                          0x0A00
#define GL_ORDER                          0x0A01
#define GL_DOMAIN                         0x0A02

/* GetPixelMap */
/*      GL_PIXEL_MAP_I_TO_I */
/*      GL_PIXEL_MAP_S_TO_S */
/*      GL_PIXEL_MAP_I_TO_R */
/*      GL_PIXEL_MAP_I_TO_G */
/*      GL_PIXEL_MAP_I_TO_B */
/*      GL_PIXEL_MAP_I_TO_A */
/*      GL_PIXEL_MAP_R_TO_R */
/*      GL_PIXEL_MAP_G_TO_G */
/*      GL_PIXEL_MAP_B_TO_B */
/*      GL_PIXEL_MAP_A_TO_A */

/* GetPointerTarget */
/*      GL_VERTEX_ARRAY_POINTER */
/*      GL_NORMAL_ARRAY_POINTER */
/*      GL_COLOR_ARRAY_POINTER */
/*      GL_INDEX_ARRAY_POINTER */
/*      GL_TEXTURE_COORD_ARRAY_POINTER */
/*      GL_EDGE_FLAG_ARRAY_POINTER */

/* GetTarget */
#define GL_CURRENT_COLOR                  0x0B00
#define GL_CURRENT_INDEX                  0x0B01
#define GL_CURRENT_NORMAL                 0x0B02
#define GL_CURRENT_TEXTURE_COORDS         0x0B03
#define GL_CURRENT_RASTER_COLOR           0x0B04
#define GL_CURRENT_RASTER_INDEX           0x0B05
#define GL_CURRENT_RASTER_TEXTURE_COORDS  0x0B06
#define GL_CURRENT_RASTER_POSITION        0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID  0x0B08
#define GL_CURRENT_RASTER_DISTANCE        0x0B09
#define GL_POINT_SMOOTH                   0x0B10
#define GL_POINT_SIZE                     0x0B11
#define GL_POINT_SIZE_RANGE               0x0B12
#define GL_POINT_SIZE_GRANULARITY         0x0B13
#define GL_LINE_SMOOTH                    0x0B20
#define GL_LINE_WIDTH                     0x0B21
#define GL_LINE_WIDTH_RANGE               0x0B22
#define GL_LINE_WIDTH_GRANULARITY         0x0B23
#define GL_LINE_STIPPLE                   0x0B24
#define GL_LINE_STIPPLE_PATTERN           0x0B25
#define GL_LINE_STIPPLE_REPEAT            0x0B26
#define GL_LIST_MODE                      0x0B30
#define GL_MAX_LIST_NESTING               0x0B31
#define GL_LIST_BASE                      0x0B32
#define GL_LIST_INDEX                     0x0B33
#define GL_POLYGON_MODE                   0x0B40
#define GL_POLYGON_SMOOTH                 0x0B41
#define GL_POLYGON_STIPPLE                0x0B42
#define GL_EDGE_FLAG                      0x0B43
#define GL_CULL_FACE                      0x0B44
#define GL_CULL_FACE_MODE                 0x0B45
#define GL_FRONT_FACE                     0x0B46
#define GL_LIGHTING                       0x0B50
#define GL_LIGHT_MODEL_LOCAL_VIEWER       0x0B51
#define GL_LIGHT_MODEL_TWO_SIDE           0x0B52
#define GL_LIGHT_MODEL_AMBIENT            0x0B53
#define GL_SHADE_MODEL                    0x0B54
#define GL_COLOR_MATERIAL_FACE            0x0B55
#define GL_COLOR_MATERIAL_PARAMETER       0x0B56
#define GL_COLOR_MATERIAL                 0x0B57
#define GL_FOG                            0x0B60
#define GL_FOG_INDEX                      0x0B61
#define GL_FOG_DENSITY                    0x0B62
#define GL_FOG_START                      0x0B63
#define GL_FOG_END                        0x0B64
#define GL_FOG_MODE                       0x0B65
#define GL_FOG_COLOR                      0x0B66
#define GL_DEPTH_RANGE                    0x0B70
#define GL_DEPTH_TEST                     0x0B71
#define GL_DEPTH_WRITEMASK                0x0B72
#define GL_DEPTH_CLEAR_VALUE              0x0B73
#define GL_DEPTH_FUNC                     0x0B74
#define GL_ACCUM_CLEAR_VALUE              0x0B80
#define GL_STENCIL_TEST                   0x0B90
#define GL_STENCIL_CLEAR_VALUE            0x0B91
#define GL_STENCIL_FUNC                   0x0B92
#define GL_STENCIL_VALUE_MASK             0x0B93
#define GL_STENCIL_FAIL                   0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS        0x0B96
#define GL_STENCIL_REF                    0x0B97
#define GL_STENCIL_WRITEMASK              0x0B98
#define GL_MATRIX_MODE                    0x0BA0
#define GL_NORMALIZE                      0x0BA1
#define GL_VIEWPORT                       0x0BA2
#define GL_MODELVIEW_STACK_DEPTH          0x0BA3
#define GL_PROJECTION_STACK_DEPTH         0x0BA4
#define GL_TEXTURE_STACK_DEPTH            0x0BA5
#define GL_MODELVIEW_MATRIX               0x0BA6
#define GL_PROJECTION_MATRIX              0x0BA7
#define GL_TEXTURE_MATRIX                 0x0BA8
#define GL_ATTRIB_STACK_DEPTH             0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH      0x0BB1
#define GL_ALPHA_TEST                     0x0BC0
#define GL_ALPHA_TEST_FUNC                0x0BC1
#define GL_ALPHA_TEST_REF                 0x0BC2
#define GL_DITHER                         0x0BD0
#define GL_BLEND_DST                      0x0BE0
#define GL_BLEND_SRC                      0x0BE1
#define GL_BLEND                          0x0BE2
#define GL_LOGIC_OP_MODE                  0x0BF0
#define GL_INDEX_LOGIC_OP                 0x0BF1
#define GL_COLOR_LOGIC_OP                 0x0BF2
#define GL_AUX_BUFFERS                    0x0C00
#define GL_DRAW_BUFFER                    0x0C01
#define GL_READ_BUFFER                    0x0C02
#define GL_SCISSOR_BOX                    0x0C10
#define GL_SCISSOR_TEST                   0x0C11
#define GL_INDEX_CLEAR_VALUE              0x0C20
#define GL_INDEX_WRITEMASK                0x0C21
#define GL_COLOR_CLEAR_VALUE              0x0C22
#define GL_COLOR_WRITEMASK                0x0C23
#define GL_INDEX_MODE                     0x0C30
#define GL_RGBA_MODE                      0x0C31
#define GL_DOUBLEBUFFER                   0x0C32
#define GL_STEREO                         0x0C33
#define GL_RENDER_MODE                    0x0C40
#define GL_PERSPECTIVE_CORRECTION_HINT    0x0C50
#define GL_POINT_SMOOTH_HINT              0x0C51
#define GL_LINE_SMOOTH_HINT               0x0C52
#define GL_POLYGON_SMOOTH_HINT            0x0C53
#define GL_FOG_HINT                       0x0C54
#define GL_TEXTURE_GEN_S                  0x0C60
#define GL_TEXTURE_GEN_T                  0x0C61
#define GL_TEXTURE_GEN_R                  0x0C62
#define GL_TEXTURE_GEN_Q                  0x0C63
#define GL_PIXEL_MAP_I_TO_I               0x0C70
#define GL_PIXEL_MAP_S_TO_S               0x0C71
#define GL_PIXEL_MAP_I_TO_R               0x0C72
#define GL_PIXEL_MAP_I_TO_G               0x0C73
#define GL_PIXEL_MAP_I_TO_B               0x0C74
#define GL_PIXEL_MAP_I_TO_A               0x0C75
#define GL_PIXEL_MAP_R_TO_R               0x0C76
#define GL_PIXEL_MAP_G_TO_G               0x0C77
#define GL_PIXEL_MAP_B_TO_B               0x0C78
#define GL_PIXEL_MAP_A_TO_A               0x0C79
#define GL_PIXEL_MAP_I_TO_I_SIZE          0x0CB0
#define GL_PIXEL_MAP_S_TO_S_SIZE          0x0CB1
#define GL_PIXEL_MAP_I_TO_R_SIZE          0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE          0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE          0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE          0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE          0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE          0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE          0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE          0x0CB9
#define GL_UNPACK_SWAP_BYTES              0x0CF0
#define GL_UNPACK_LSB_FIRST               0x0CF1
#define GL_UNPACK_ROW_LENGTH              0x0CF2
#define GL_UNPACK_SKIP_ROWS               0x0CF3
#define GL_UNPACK_SKIP_PIXELS             0x0CF4
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_SWAP_BYTES                0x0D00
#define GL_PACK_LSB_FIRST                 0x0D01
#define GL_PACK_ROW_LENGTH                0x0D02
#define GL_PACK_SKIP_ROWS                 0x0D03
#define GL_PACK_SKIP_PIXELS               0x0D04
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_MAP_COLOR                      0x0D10
#define GL_MAP_STENCIL                    0x0D11
#define GL_INDEX_SHIFT                    0x0D12
#define GL_INDEX_OFFSET                   0x0D13
#define GL_RED_SCALE                      0x0D14
#define GL_RED_BIAS                       0x0D15
#define GL_ZOOM_X                         0x0D16
#define GL_ZOOM_Y                         0x0D17
#define GL_GREEN_SCALE                    0x0D18
#define GL_GREEN_BIAS                     0x0D19
#define GL_BLUE_SCALE                     0x0D1A
#define GL_BLUE_BIAS                      0x0D1B
#define GL_ALPHA_SCALE                    0x0D1C
#define GL_ALPHA_BIAS                     0x0D1D
#define GL_DEPTH_SCALE                    0x0D1E
#define GL_DEPTH_BIAS                     0x0D1F
#define GL_MAX_EVAL_ORDER                 0x0D30
#define GL_MAX_LIGHTS                     0x0D31
#define GL_MAX_CLIP_PLANES                0x0D32
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_PIXEL_MAP_TABLE            0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH         0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH      0x0D36
#define GL_MAX_NAME_STACK_DEPTH           0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH     0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH        0x0D39
#define GL_MAX_VIEWPORT_DIMS              0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH  0x0D3B
#define GL_SUBPIXEL_BITS                  0x0D50
#define GL_INDEX_BITS                     0x0D51
#define GL_RED_BITS                       0x0D52
#define GL_GREEN_BITS                     0x0D53
#define GL_BLUE_BITS                      0x0D54
#define GL_ALPHA_BITS                     0x0D55
#define GL_DEPTH_BITS                     0x0D56
#define GL_STENCIL_BITS                   0x0D57
#define GL_ACCUM_RED_BITS                 0x0D58
#define GL_ACCUM_GREEN_BITS               0x0D59
#define GL_ACCUM_BLUE_BITS                0x0D5A
#define GL_ACCUM_ALPHA_BITS               0x0D5B
#define GL_NAME_STACK_DEPTH               0x0D70
#define GL_AUTO_NORMAL                    0x0D80
#define GL_MAP1_COLOR_4                   0x0D90
#define GL_MAP1_INDEX                     0x0D91
#define GL_MAP1_NORMAL                    0x0D92
#define GL_MAP1_TEXTURE_COORD_1           0x0D93
#define GL_MAP1_TEXTURE_COORD_2           0x0D94
#define GL_MAP1_TEXTURE_COORD_3           0x0D95
#define GL_MAP1_TEXTURE_COORD_4           0x0D96
#define GL_MAP1_VERTEX_3                  0x0D97
#define GL_MAP1_VERTEX_4                  0x0D98
#define GL_MAP2_COLOR_4                   0x0DB0
#define GL_MAP2_INDEX                     0x0DB1
#define GL_MAP2_NORMAL                    0x0DB2
#define GL_MAP2_TEXTURE_COORD_1           0x0DB3
#define GL_MAP2_TEXTURE_COORD_2           0x0DB4
#define GL_MAP2_TEXTURE_COORD_3           0x0DB5
#define GL_MAP2_TEXTURE_COORD_4           0x0DB6
#define GL_MAP2_VERTEX_3                  0x0DB7
#define GL_MAP2_VERTEX_4                  0x0DB8
#define GL_MAP1_GRID_DOMAIN               0x0DD0
#define GL_MAP1_GRID_SEGMENTS             0x0DD1
#define GL_MAP2_GRID_DOMAIN               0x0DD2
#define GL_MAP2_GRID_SEGMENTS             0x0DD3
#define GL_TEXTURE_1D                     0x0DE0
#define GL_TEXTURE_2D                     0x0DE1
#define GL_FEEDBACK_BUFFER_POINTER        0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE           0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE           0x0DF2
#define GL_SELECTION_BUFFER_POINTER       0x0DF3
#define GL_SELECTION_BUFFER_SIZE          0x0DF4
/*      GL_TEXTURE_BINDING_1D */
/*      GL_TEXTURE_BINDING_2D */
/*      GL_VERTEX_ARRAY */
/*      GL_NORMAL_ARRAY */
/*      GL_COLOR_ARRAY */
/*      GL_INDEX_ARRAY */
/*      GL_TEXTURE_COORD_ARRAY */
/*      GL_EDGE_FLAG_ARRAY */
/*      GL_VERTEX_ARRAY_SIZE */
/*      GL_VERTEX_ARRAY_TYPE */
/*      GL_VERTEX_ARRAY_STRIDE */
/*      GL_NORMAL_ARRAY_TYPE */
/*      GL_NORMAL_ARRAY_STRIDE */
/*      GL_COLOR_ARRAY_SIZE */
/*      GL_COLOR_ARRAY_TYPE */
/*      GL_COLOR_ARRAY_STRIDE */
/*      GL_INDEX_ARRAY_TYPE */
/*      GL_INDEX_ARRAY_STRIDE */
/*      GL_TEXTURE_COORD_ARRAY_SIZE */
/*      GL_TEXTURE_COORD_ARRAY_TYPE */
/*      GL_TEXTURE_COORD_ARRAY_STRIDE */
/*      GL_EDGE_FLAG_ARRAY_STRIDE */
/*      GL_POLYGON_OFFSET_FACTOR */
/*      GL_POLYGON_OFFSET_UNITS */

/* GetTextureParameter */
/*      GL_TEXTURE_MAG_FILTER */
/*      GL_TEXTURE_MIN_FILTER */
/*      GL_TEXTURE_WRAP_S */
/*      GL_TEXTURE_WRAP_T */
#define GL_TEXTURE_WIDTH                  0x1000
#define GL_TEXTURE_HEIGHT                 0x1001
#define GL_TEXTURE_INTERNAL_FORMAT        0x1003
#define GL_TEXTURE_BORDER_COLOR           0x1004
#define GL_TEXTURE_BORDER                 0x1005
/*      GL_TEXTURE_RED_SIZE */
/*      GL_TEXTURE_GREEN_SIZE */
/*      GL_TEXTURE_BLUE_SIZE */
/*      GL_TEXTURE_ALPHA_SIZE */
/*      GL_TEXTURE_LUMINANCE_SIZE */
/*      GL_TEXTURE_INTENSITY_SIZE */
/*      GL_TEXTURE_PRIORITY */
/*      GL_TEXTURE_RESIDENT */

/* HintMode */
#define GL_DONT_CARE                      0x1100
#define GL_FASTEST                        0x1101
#define GL_NICEST                         0x1102

/* HintTarget */
/*      GL_PERSPECTIVE_CORRECTION_HINT */
/*      GL_POINT_SMOOTH_HINT */
/*      GL_LINE_SMOOTH_HINT */
/*      GL_POLYGON_SMOOTH_HINT */
/*      GL_FOG_HINT */
/*      GL_PHONG_HINT */

/* IndexPointerType */
/*      GL_SHORT */
/*      GL_INT */
/*      GL_FLOAT */
/*      GL_DOUBLE */

/* LightModelParameter */
/*      GL_LIGHT_MODEL_AMBIENT */
/*      GL_LIGHT_MODEL_LOCAL_VIEWER */
/*      GL_LIGHT_MODEL_TWO_SIDE */

/* LightName */
#define GL_LIGHT0                         0x4000
#define GL_LIGHT1                         0x4001
#define GL_LIGHT2                         0x4002
#define GL_LIGHT3                         0x4003
#define GL_LIGHT4                         0x4004
#define GL_LIGHT5                         0x4005
#define GL_LIGHT6                         0x4006
#define GL_LIGHT7                         0x4007

/* LightParameter */
#define GL_AMBIENT                        0x1200
#define GL_DIFFUSE                        0x1201
#define GL_SPECULAR                       0x1202
#define GL_POSITION                       0x1203
#define GL_SPOT_DIRECTION                 0x1204
#define GL_SPOT_EXPONENT                  0x1205
#define GL_SPOT_CUTOFF                    0x1206
#define GL_CONSTANT_ATTENUATION           0x1207
#define GL_LINEAR_ATTENUATION             0x1208
#define GL_QUADRATIC_ATTENUATION          0x1209

/* InterleavedArrays */
/*      GL_V2F */
/*      GL_V3F */
/*      GL_C4UB_V2F */
/*      GL_C4UB_V3F */
/*      GL_C3F_V3F */
/*      GL_N3F_V3F */
/*      GL_C4F_N3F_V3F */
/*      GL_T2F_V3F */
/*      GL_T4F_V4F */
/*      GL_T2F_C4UB_V3F */
/*      GL_T2F_C3F_V3F */
/*      GL_T2F_N3F_V3F */
/*      GL_T2F_C4F_N3F_V3F */
/*      GL_T4F_C4F_N3F_V4F */

/* ListMode */
#define GL_COMPILE                        0x1300
#define GL_COMPILE_AND_EXECUTE            0x1301

/* ListNameType */
/*      GL_BYTE */
/*      GL_UNSIGNED_BYTE */
/*      GL_SHORT */
/*      GL_UNSIGNED_SHORT */
/*      GL_INT */
/*      GL_UNSIGNED_INT */
/*      GL_FLOAT */
/*      GL_2_BYTES */
/*      GL_3_BYTES */
/*      GL_4_BYTES */

/* LogicOp */
#define GL_CLEAR                          0x1500
#define GL_AND                            0x1501
#define GL_AND_REVERSE                    0x1502
#define GL_COPY                           0x1503
#define GL_AND_INVERTED                   0x1504
#define GL_NOOP                           0x1505
#define GL_XOR                            0x1506
#define GL_OR                             0x1507
#define GL_NOR                            0x1508
#define GL_EQUIV                          0x1509
#define GL_INVERT                         0x150A
#define GL_OR_REVERSE                     0x150B
#define GL_COPY_INVERTED                  0x150C
#define GL_OR_INVERTED                    0x150D
#define GL_NAND                           0x150E
#define GL_SET                            0x150F

/* MapTarget */
/*      GL_MAP1_COLOR_4 */
/*      GL_MAP1_INDEX */
/*      GL_MAP1_NORMAL */
/*      GL_MAP1_TEXTURE_COORD_1 */
/*      GL_MAP1_TEXTURE_COORD_2 */
/*      GL_MAP1_TEXTURE_COORD_3 */
/*      GL_MAP1_TEXTURE_COORD_4 */
/*      GL_MAP1_VERTEX_3 */
/*      GL_MAP1_VERTEX_4 */
/*      GL_MAP2_COLOR_4 */
/*      GL_MAP2_INDEX */
/*      GL_MAP2_NORMAL */
/*      GL_MAP2_TEXTURE_COORD_1 */
/*      GL_MAP2_TEXTURE_COORD_2 */
/*      GL_MAP2_TEXTURE_COORD_3 */
/*      GL_MAP2_TEXTURE_COORD_4 */
/*      GL_MAP2_VERTEX_3 */
/*      GL_MAP2_VERTEX_4 */

/* MaterialFace */
/*      GL_FRONT */
/*      GL_BACK */
/*      GL_FRONT_AND_BACK */

/* MaterialParameter */
#define GL_EMISSION                       0x1600
#define GL_SHININESS                      0x1601
#define GL_AMBIENT_AND_DIFFUSE            0x1602
#define GL_COLOR_INDEXES                  0x1603
/*      GL_AMBIENT */
/*      GL_DIFFUSE */
/*      GL_SPECULAR */

/* MatrixMode */
#define GL_MODELVIEW                      0x1700
#define GL_PROJECTION                     0x1701
#define GL_TEXTURE                        0x1702

/* MeshMode1 */
/*      GL_POINT */
/*      GL_LINE */

/* MeshMode2 */
/*      GL_POINT */
/*      GL_LINE */
/*      GL_FILL */

/* NormalPointerType */
/*      GL_BYTE */
/*      GL_SHORT */
/*      GL_INT */
/*      GL_FLOAT */
/*      GL_DOUBLE */

/* PixelCopyType */
#define GL_COLOR                          0x1800
#define GL_DEPTH                          0x1801
#define GL_STENCIL                        0x1802

/* PixelFormat */
#define GL_COLOR_INDEX                    0x1900
#define GL_STENCIL_INDEX                  0x1901
#define GL_DEPTH_COMPONENT                0x1902
#define GL_RED                            0x1903
#define GL_GREEN                          0x1904
#define GL_BLUE                           0x1905
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A

/* PixelMap */
/*      GL_PIXEL_MAP_I_TO_I */
/*      GL_PIXEL_MAP_S_TO_S */
/*      GL_PIXEL_MAP_I_TO_R */
/*      GL_PIXEL_MAP_I_TO_G */
/*      GL_PIXEL_MAP_I_TO_B */
/*      GL_PIXEL_MAP_I_TO_A */
/*      GL_PIXEL_MAP_R_TO_R */
/*      GL_PIXEL_MAP_G_TO_G */
/*      GL_PIXEL_MAP_B_TO_B */
/*      GL_PIXEL_MAP_A_TO_A */

/* PixelStore */
/*      GL_UNPACK_SWAP_BYTES */
/*      GL_UNPACK_LSB_FIRST */
/*      GL_UNPACK_ROW_LENGTH */
/*      GL_UNPACK_SKIP_ROWS */
/*      GL_UNPACK_SKIP_PIXELS */
/*      GL_UNPACK_ALIGNMENT */
/*      GL_PACK_SWAP_BYTES */
/*      GL_PACK_LSB_FIRST */
/*      GL_PACK_ROW_LENGTH */
/*      GL_PACK_SKIP_ROWS */
/*      GL_PACK_SKIP_PIXELS */
/*      GL_PACK_ALIGNMENT */

/* PixelTransfer */
/*      GL_MAP_COLOR */
/*      GL_MAP_STENCIL */
/*      GL_INDEX_SHIFT */
/*      GL_INDEX_OFFSET */
/*      GL_RED_SCALE */
/*      GL_RED_BIAS */
/*      GL_GREEN_SCALE */
/*      GL_GREEN_BIAS */
/*      GL_BLUE_SCALE */
/*      GL_BLUE_BIAS */
/*      GL_ALPHA_SCALE */
/*      GL_ALPHA_BIAS */
/*      GL_DEPTH_SCALE */
/*      GL_DEPTH_BIAS */

/* PixelType */
#define GL_BITMAP                         0x1A00
/*      GL_BYTE */
/*      GL_UNSIGNED_BYTE */
/*      GL_SHORT */
/*      GL_UNSIGNED_SHORT */
/*      GL_INT */
/*      GL_UNSIGNED_INT */
/*      GL_FLOAT */

/* PolygonMode */
#define GL_POINT                          0x1B00
#define GL_LINE                           0x1B01
#define GL_FILL                           0x1B02

/* ReadBufferMode */
/*      GL_FRONT_LEFT */
/*      GL_FRONT_RIGHT */
/*      GL_BACK_LEFT */
/*      GL_BACK_RIGHT */
/*      GL_FRONT */
/*      GL_BACK */
/*      GL_LEFT */
/*      GL_RIGHT */
/*      GL_AUX0 */
/*      GL_AUX1 */
/*      GL_AUX2 */
/*      GL_AUX3 */

/* RenderingMode */
#define GL_RENDER                         0x1C00
#define GL_FEEDBACK                       0x1C01
#define GL_SELECT                         0x1C02

/* ShadingModel */
#define GL_FLAT                           0x1D00
#define GL_SMOOTH                         0x1D01


/* StencilFunction */
/*      GL_NEVER */
/*      GL_LESS */
/*      GL_EQUAL */
/*      GL_LEQUAL */
/*      GL_GREATER */
/*      GL_NOTEQUAL */
/*      GL_GEQUAL */
/*      GL_ALWAYS */

/* StencilOp */
/*      GL_ZERO */
#define GL_KEEP                           0x1E00
#define GL_REPLACE                        0x1E01
#define GL_INCR                           0x1E02
#define GL_DECR                           0x1E03
/*      GL_INVERT */

/* StringName */
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03

/* TextureCoordName */
#define GL_S                              0x2000
#define GL_T                              0x2001
#define GL_R                              0x2002
#define GL_Q                              0x2003

/* TexCoordPointerType */
/*      GL_SHORT */
/*      GL_INT */
/*      GL_FLOAT */
/*      GL_DOUBLE */

/* TextureEnvMode */
#define GL_MODULATE                       0x2100
#define GL_DECAL                          0x2101
/*      GL_BLEND */
/*      GL_REPLACE */

/* TextureEnvParameter */
#define GL_TEXTURE_ENV_MODE               0x2200
#define GL_TEXTURE_ENV_COLOR              0x2201

/* TextureEnvTarget */
#define GL_TEXTURE_ENV                    0x2300

/* TextureGenMode */
#define GL_EYE_LINEAR                     0x2400
#define GL_OBJECT_LINEAR                  0x2401
#define GL_SPHERE_MAP                     0x2402

/* TextureGenParameter */
#define GL_TEXTURE_GEN_MODE               0x2500
#define GL_OBJECT_PLANE                   0x2501
#define GL_EYE_PLANE                      0x2502

/* TextureMagFilter */
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601

/* TextureMinFilter */
/*      GL_NEAREST */
/*      GL_LINEAR */
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703

/* TextureParameterName */
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
/*      GL_TEXTURE_BORDER_COLOR */
/*      GL_TEXTURE_PRIORITY */

/* TextureTarget */
/*      GL_TEXTURE_1D */
/*      GL_TEXTURE_2D */
/*      GL_PROXY_TEXTURE_1D */
/*      GL_PROXY_TEXTURE_2D */

/* TextureWrapMode */
#define GL_CLAMP                          0x2900
#define GL_REPEAT                         0x2901

/* VertexPointerType */
/*      GL_SHORT */
/*      GL_INT */
/*      GL_FLOAT */
/*      GL_DOUBLE */

/* ClientAttribMask */
#define GL_CLIENT_PIXEL_STORE_BIT         0x00000001
#define GL_CLIENT_VERTEX_ARRAY_BIT        0x00000002
#define GL_CLIENT_ALL_ATTRIB_BITS         0xffffffff

/* polygon_offset */
#define GL_POLYGON_OFFSET_FACTOR          0x8038
#define GL_POLYGON_OFFSET_UNITS           0x2A00
#define GL_POLYGON_OFFSET_POINT           0x2A01
#define GL_POLYGON_OFFSET_LINE            0x2A02
#define GL_POLYGON_OFFSET_FILL            0x8037

/* texture */
#define GL_ALPHA4                         0x803B
#define GL_ALPHA8                         0x803C
#define GL_ALPHA12                        0x803D
#define GL_ALPHA16                        0x803E
#define GL_LUMINANCE4                     0x803F
#define GL_LUMINANCE8                     0x8040
#define GL_LUMINANCE12                    0x8041
#define GL_LUMINANCE16                    0x8042
#define GL_LUMINANCE4_ALPHA4              0x8043
#define GL_LUMINANCE6_ALPHA2              0x8044
#define GL_LUMINANCE8_ALPHA8              0x8045
#define GL_LUMINANCE12_ALPHA4             0x8046
#define GL_LUMINANCE12_ALPHA12            0x8047
#define GL_LUMINANCE16_ALPHA16            0x8048
#define GL_INTENSITY                      0x8049
#define GL_INTENSITY4                     0x804A
#define GL_INTENSITY8                     0x804B
#define GL_INTENSITY12                    0x804C
#define GL_INTENSITY16                    0x804D
#define GL_R3_G3_B2                       0x2A10
#define GL_RGB4                           0x804F
#define GL_RGB5                           0x8050
#define GL_RGB8                           0x8051
#define GL_RGB10                          0x8052
#define GL_RGB12                          0x8053
#define GL_RGB16                          0x8054
#define GL_RGBA2                          0x8055
#define GL_RGBA4                          0x8056
#define GL_RGB5_A1                        0x8057
#define GL_RGBA8                          0x8058
#define GL_RGB10_A2                       0x8059
#define GL_RGBA12                         0x805A
#define GL_RGBA16                         0x805B
#define GL_TEXTURE_RED_SIZE               0x805C
#define GL_TEXTURE_GREEN_SIZE             0x805D
#define GL_TEXTURE_BLUE_SIZE              0x805E
#define GL_TEXTURE_ALPHA_SIZE             0x805F
#define GL_TEXTURE_LUMINANCE_SIZE         0x8060
#define GL_TEXTURE_INTENSITY_SIZE         0x8061
#define GL_PROXY_TEXTURE_1D               0x8063
#define GL_PROXY_TEXTURE_2D               0x8064

/* texture_object */
#define GL_TEXTURE_PRIORITY               0x8066
#define GL_TEXTURE_RESIDENT               0x8067
#define GL_TEXTURE_BINDING_1D             0x8068
#define GL_TEXTURE_BINDING_2D             0x8069

/* vertex_array */
#define GL_VERTEX_ARRAY                   0x8074
#define GL_NORMAL_ARRAY                   0x8075
#define GL_COLOR_ARRAY                    0x8076
#define GL_INDEX_ARRAY                    0x8077
#define GL_TEXTURE_COORD_ARRAY            0x8078
#define GL_EDGE_FLAG_ARRAY                0x8079
#define GL_VERTEX_ARRAY_SIZE              0x807A
#define GL_VERTEX_ARRAY_TYPE              0x807B
#define GL_VERTEX_ARRAY_STRIDE            0x807C
#define GL_NORMAL_ARRAY_TYPE              0x807E
#define GL_NORMAL_ARRAY_STRIDE            0x807F
#define GL_COLOR_ARRAY_SIZE               0x8081
#define GL_COLOR_ARRAY_TYPE               0x8082
#define GL_COLOR_ARRAY_STRIDE             0x8083
#define GL_INDEX_ARRAY_TYPE               0x8085
#define GL_INDEX_ARRAY_STRIDE             0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE       0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE       0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE     0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE         0x808C
#define GL_VERTEX_ARRAY_POINTER           0x808E
#define GL_NORMAL_ARRAY_POINTER           0x808F
#define GL_COLOR_ARRAY_POINTER            0x8090
#define GL_INDEX_ARRAY_POINTER            0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER    0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER        0x8093
#define GL_V2F                            0x2A20
#define GL_V3F                            0x2A21
#define GL_C4UB_V2F                       0x2A22
#define GL_C4UB_V3F                       0x2A23
#define GL_C3F_V3F                        0x2A24
#define GL_N3F_V3F                        0x2A25
#define GL_C4F_N3F_V3F                    0x2A26
#define GL_T2F_V3F                        0x2A27
#define GL_T4F_V4F                        0x2A28
#define GL_T2F_C4UB_V3F                   0x2A29
#define GL_T2F_C3F_V3F                    0x2A2A
#define GL_T2F_N3F_V3F                    0x2A2B
#define GL_T2F_C4F_N3F_V3F                0x2A2C
#define GL_T4F_C4F_N3F_V4F                0x2A2D

/* Extensions */
#define GL_EXT_vertex_array               1
#define GL_EXT_bgra                       1
#define GL_EXT_paletted_texture           1
#define GL_WIN_swap_hint                  1
#define GL_WIN_draw_range_elements        1
// #define GL_WIN_phong_shading              1
// #define GL_WIN_specular_fog               1

/* EXT_vertex_array */
#define GL_VERTEX_ARRAY_EXT               0x8074
#define GL_NORMAL_ARRAY_EXT               0x8075
#define GL_COLOR_ARRAY_EXT                0x8076
#define GL_INDEX_ARRAY_EXT                0x8077
#define GL_TEXTURE_COORD_ARRAY_EXT        0x8078
#define GL_EDGE_FLAG_ARRAY_EXT            0x8079
#define GL_VERTEX_ARRAY_SIZE_EXT          0x807A
#define GL_VERTEX_ARRAY_TYPE_EXT          0x807B
#define GL_VERTEX_ARRAY_STRIDE_EXT        0x807C
#define GL_VERTEX_ARRAY_COUNT_EXT         0x807D
#define GL_NORMAL_ARRAY_TYPE_EXT          0x807E
#define GL_NORMAL_ARRAY_STRIDE_EXT        0x807F
#define GL_NORMAL_ARRAY_COUNT_EXT         0x8080
#define GL_COLOR_ARRAY_SIZE_EXT           0x8081
#define GL_COLOR_ARRAY_TYPE_EXT           0x8082
#define GL_COLOR_ARRAY_STRIDE_EXT         0x8083
#define GL_COLOR_ARRAY_COUNT_EXT          0x8084
#define GL_INDEX_ARRAY_TYPE_EXT           0x8085
#define GL_INDEX_ARRAY_STRIDE_EXT         0x8086
#define GL_INDEX_ARRAY_COUNT_EXT          0x8087
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT   0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT   0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT 0x808A
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT  0x808B
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT     0x808C
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT      0x808D
#define GL_VERTEX_ARRAY_POINTER_EXT       0x808E
#define GL_NORMAL_ARRAY_POINTER_EXT       0x808F
#define GL_COLOR_ARRAY_POINTER_EXT        0x8090
#define GL_INDEX_ARRAY_POINTER_EXT        0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT    0x8093
#define GL_DOUBLE_EXT                     GL_DOUBLE

/* EXT_bgra */
#define GL_BGR_EXT                        0x80E0
#define GL_BGRA_EXT                       0x80E1

/* EXT_paletted_texture */

/* These must match the GL_COLOR_TABLE_*_SGI enumerants */
#define GL_COLOR_TABLE_FORMAT_EXT         0x80D8
#define GL_COLOR_TABLE_WIDTH_EXT          0x80D9
#define GL_COLOR_TABLE_RED_SIZE_EXT       0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_EXT     0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_EXT      0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_EXT     0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_EXT 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_EXT 0x80DF

#define GL_COLOR_INDEX1_EXT               0x80E2
#define GL_COLOR_INDEX2_EXT               0x80E3
#define GL_COLOR_INDEX4_EXT               0x80E4
#define GL_COLOR_INDEX8_EXT               0x80E5
#define GL_COLOR_INDEX12_EXT              0x80E6
#define GL_COLOR_INDEX16_EXT              0x80E7

/* WIN_draw_range_elements */
#define GL_MAX_ELEMENTS_VERTICES_WIN      0x80E8
#define GL_MAX_ELEMENTS_INDICES_WIN       0x80E9

/* WIN_phong_shading */
#define GL_PHONG_WIN                      0x80EA 
#define GL_PHONG_HINT_WIN                 0x80EB 

/* WIN_specular_fog */
#define GL_FOG_SPECULAR_TEXTURE_WIN       0x80EC

/* For compatibility with OpenGL v1.0 */
#define GL_LOGIC_OP GL_INDEX_LOGIC_OP
#define GL_TEXTURE_COMPONENTS GL_TEXTURE_INTERNAL_FORMAT



/* OpenGL Multi Texturing Defines, we don't actually use them */

#define GL_TEXTURE0_ARB					0x84C0
#define GL_TEXTURE1_ARB					0x84C1
#define GL_TEXTURE2_ARB					0x84C2
#define GL_TEXTURE3_ARB					0x84C3
#define GL_TEXTURE4_ARB					0x84C4
#define GL_TEXTURE5_ARB					0x84C5
#define GL_TEXTURE6_ARB					0x84C6
#define GL_TEXTURE7_ARB					0x84C7
#define GL_TEXTURE8_ARB					0x84C8
#define GL_TEXTURE9_ARB					0x84C9
#define GL_TEXTURE10_ARB				0x84CA
#define GL_TEXTURE11_ARB				0x84CB
#define GL_TEXTURE12_ARB				0x84CC
#define GL_TEXTURE13_ARB				0x84CD
#define GL_TEXTURE14_ARB				0x84CE
#define GL_TEXTURE15_ARB				0x84CF
#define GL_TEXTURE16_ARB				0x84D0
#define GL_TEXTURE17_ARB				0x84D1
#define GL_TEXTURE18_ARB				0x84D2
#define GL_TEXTURE19_ARB				0x84D3
#define GL_TEXTURE20_ARB				0x84D4
#define GL_TEXTURE21_ARB				0x84D5
#define GL_TEXTURE22_ARB				0x84D6
#define GL_TEXTURE23_ARB				0x84D7
#define GL_TEXTURE24_ARB				0x84D8
#define GL_TEXTURE25_ARB				0x84D9
#define GL_TEXTURE26_ARB				0x84DA
#define GL_TEXTURE27_ARB				0x84DB
#define GL_TEXTURE28_ARB				0x84DC
#define GL_TEXTURE29_ARB				0x84DD
#define GL_TEXTURE30_ARB				0x84DE
#define GL_TEXTURE31_ARB				0x84DF
#define GL_ACTIVE_TEXTURE_ARB			0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB	0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB		0x84E2

#define GLAPIENTRY __stdcall

// ******************************************************************

namespace OpenGL {
	bool CheckBoundTexture(GLuint texture);
	bool CheckBlendFunc(GLenum src, GLenum dst);
	bool CheckCap(GLenum cap, GLboolean state);
	bool CheckTexEnv(GLint param);
	bool IsActive();
	void ShutdownTex1ARB();

	extern Fear::OpenGLState* mState;
};


void glBegin(GLenum mode);
void glBindTexture(GLenum target, GLuint texture);
void glBlendFunc(GLenum sfactor, GLenum dfactor);
void glCallLists(GLsizei n, GLenum type_, const GLvoid* lists);
void glCallList(GLuint list);
void glColor3ub(GLubyte red, GLubyte green, GLubyte blue);
void glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
void glColorMask(GLboolean r, GLboolean g, GLboolean b, GLboolean a);
void glDeleteLists(GLuint list, GLsizei range);
void glDeleteTextures(GLsizei n, const GLuint* textures);
void glDepthFunc(GLenum func);
void glDepthMask(GLboolean flag);
void glDepthRange(GLclampd znear, GLclampd zfar);
void glDisable(GLenum cap);
void glDrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum pixeltype, const GLvoid* pixels);
void glEnable(GLenum cap);
void glEnd();
void glEndList();
void glFlush();
GLuint glGenLists(GLsizei range);
void glGenTextures(GLsizei n, GLuint* textures);
GLenum glGetError();
void glGetIntegerv(GLenum pname, GLint* params);
void glGetTexEnviv(GLenum target, GLenum pname, GLint* params);
void glListBase(GLuint base);
void glLoadIdentity();
void glMatrixMode(GLenum mode);
void glNewList(GLuint list, GLenum mode);
void glPixelMapusv(GLenum map, GLsizei mapsize, const GLushort* values);
void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble znear, GLdouble zfar);
void glPixelTransferi(GLenum pname, GLint param);
void glPopAttrib();
void glPopMatrix();
void glPushAttrib(GLenum attrib);
void glPushMatrix();
void glRasterPos2i(GLint x, GLint y);
void glReadBuffer(GLenum mode);
void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum pixeltype, const GLvoid* pixels);
void glScalef(GLfloat x, GLfloat y, GLfloat z);
void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
void glTexCoord2f(GLfloat s, GLfloat t);
void glTexCoord2fv(Vector2f* v);
void glTexEnvi(GLenum target, GLenum pname, GLint param);
void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type_, const GLvoid* pixels);
void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type_, const GLvoid* pixels);
void glTexParameteri(GLenum target, GLenum pname, GLint param);
void glTranslatef(GLfloat x, GLfloat y, GLfloat z);
void glVertex2f(GLfloat x, GLfloat y);
void glVertex2fv(Vector2f* v);
void glVertex2i(GLint x, GLint y);
void glVertex3f(GLfloat x, GLfloat y, GLfloat z);

#endif // __OPENGL_H__