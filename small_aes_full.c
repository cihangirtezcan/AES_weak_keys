#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
typedef unsigned __int64 bit64;
typedef unsigned long bit32;
typedef unsigned char bit8;
typedef unsigned char UINT8;
#include "small_aes.h"
FILE *fp, *fp2;
FILE *f1, *f2, *f3, *f4;
//#define datasize 0x04000000
//#define datasize 100
//#define data 10000
#define datasize 0x800000
#define data 0x100000000
__int64 data2=0;
int block1 = 0, block2 = 0;
bit64 input1[datasize], input2[datasize], integral = 0;
bit64 difference[65536][4] = { 0 };
//bit64 mc[65536] = { 0 };
//bit64 imc[65536] = { 0 };

//bit64 data = 0xffffffff;
//#define data 0x100000000
#define ROTR(x,n) (((x)>>(n))|((x)<<(64-(n))))
#define ROTL(x,n) (((x)<<(n))|((x)>>(64-(n))))
#define ROTL16(x,n) (((x)<<(n))|((x)>>(16-(n))))&0xffff
/// REAL AES //////
bit32 table0[256] = { 0xc66363a5, 0xf87c7c84, 0xee777799, 0xf67b7b8d, 0xfff2f20d, 0xd66b6bbd, 0xde6f6fb1, 0x91c5c554, 0x60303050, 0x2010103, 0xce6767a9, 0x562b2b7d, 0xe7fefe19, 0xb5d7d762, 0x4dababe6, 0xec76769a, 0x8fcaca45, 0x1f82829d, 0x89c9c940, 0xfa7d7d87, 0xeffafa15, 0xb25959eb, 0x8e4747c9, 0xfbf0f00b, 0x41adadec, 0xb3d4d467, 0x5fa2a2fd, 0x45afafea, 0x239c9cbf, 0x53a4a4f7, 0xe4727296, 0x9bc0c05b, 0x75b7b7c2, 0xe1fdfd1c, 0x3d9393ae, 0x4c26266a, 0x6c36365a, 0x7e3f3f41, 0xf5f7f702, 0x83cccc4f, 0x6834345c, 0x51a5a5f4, 0xd1e5e534, 0xf9f1f108, 0xe2717193, 0xabd8d873, 0x62313153, 0x2a15153f, 0x804040c, 0x95c7c752, 0x46232365, 0x9dc3c35e, 0x30181828, 0x379696a1, 0xa05050f, 0x2f9a9ab5, 0xe070709, 0x24121236, 0x1b80809b, 0xdfe2e23d, 0xcdebeb26, 0x4e272769, 0x7fb2b2cd, 0xea75759f, 0x1209091b, 0x1d83839e, 0x582c2c74, 0x341a1a2e, 0x361b1b2d, 0xdc6e6eb2, 0xb45a5aee, 0x5ba0a0fb, 0xa45252f6, 0x763b3b4d, 0xb7d6d661, 0x7db3b3ce, 0x5229297b, 0xdde3e33e, 0x5e2f2f71, 0x13848497, 0xa65353f5, 0xb9d1d168, 0x0, 0xc1eded2c, 0x40202060, 0xe3fcfc1f, 0x79b1b1c8, 0xb65b5bed, 0xd46a6abe, 0x8dcbcb46, 0x67bebed9, 0x7239394b, 0x944a4ade, 0x984c4cd4, 0xb05858e8, 0x85cfcf4a, 0xbbd0d06b, 0xc5efef2a, 0x4faaaae5, 0xedfbfb16, 0x864343c5, 0x9a4d4dd7, 0x66333355, 0x11858594, 0x8a4545cf, 0xe9f9f910, 0x4020206, 0xfe7f7f81, 0xa05050f0, 0x783c3c44, 0x259f9fba, 0x4ba8a8e3, 0xa25151f3, 0x5da3a3fe, 0x804040c0, 0x58f8f8a, 0x3f9292ad, 0x219d9dbc, 0x70383848, 0xf1f5f504, 0x63bcbcdf, 0x77b6b6c1, 0xafdada75, 0x42212163, 0x20101030, 0xe5ffff1a, 0xfdf3f30e, 0xbfd2d26d, 0x81cdcd4c, 0x180c0c14, 0x26131335, 0xc3ecec2f, 0xbe5f5fe1, 0x359797a2, 0x884444cc, 0x2e171739, 0x93c4c457, 0x55a7a7f2, 0xfc7e7e82, 0x7a3d3d47, 0xc86464ac, 0xba5d5de7, 0x3219192b, 0xe6737395, 0xc06060a0, 0x19818198, 0x9e4f4fd1, 0xa3dcdc7f, 0x44222266, 0x542a2a7e, 0x3b9090ab, 0xb888883, 0x8c4646ca, 0xc7eeee29, 0x6bb8b8d3, 0x2814143c, 0xa7dede79, 0xbc5e5ee2, 0x160b0b1d, 0xaddbdb76, 0xdbe0e03b, 0x64323256, 0x743a3a4e, 0x140a0a1e, 0x924949db, 0xc06060a, 0x4824246c, 0xb85c5ce4, 0x9fc2c25d, 0xbdd3d36e, 0x43acacef, 0xc46262a6, 0x399191a8, 0x319595a4, 0xd3e4e437, 0xf279798b, 0xd5e7e732, 0x8bc8c843, 0x6e373759, 0xda6d6db7, 0x18d8d8c, 0xb1d5d564, 0x9c4e4ed2, 0x49a9a9e0, 0xd86c6cb4, 0xac5656fa, 0xf3f4f407, 0xcfeaea25, 0xca6565af, 0xf47a7a8e, 0x47aeaee9, 0x10080818, 0x6fbabad5, 0xf0787888, 0x4a25256f, 0x5c2e2e72, 0x381c1c24, 0x57a6a6f1, 0x73b4b4c7, 0x97c6c651, 0xcbe8e823, 0xa1dddd7c, 0xe874749c, 0x3e1f1f21, 0x964b4bdd, 0x61bdbddc, 0xd8b8b86, 0xf8a8a85, 0xe0707090, 0x7c3e3e42, 0x71b5b5c4, 0xcc6666aa, 0x904848d8, 0x6030305, 0xf7f6f601, 0x1c0e0e12, 0xc26161a3, 0x6a35355f, 0xae5757f9, 0x69b9b9d0, 0x17868691, 0x99c1c158, 0x3a1d1d27, 0x279e9eb9, 0xd9e1e138, 0xebf8f813, 0x2b9898b3, 0x22111133, 0xd26969bb, 0xa9d9d970, 0x78e8e89, 0x339494a7, 0x2d9b9bb6, 0x3c1e1e22, 0x15878792, 0xc9e9e920, 0x87cece49, 0xaa5555ff, 0x50282878, 0xa5dfdf7a, 0x38c8c8f, 0x59a1a1f8, 0x9898980, 0x1a0d0d17, 0x65bfbfda, 0xd7e6e631, 0x844242c6, 0xd06868b8, 0x824141c3, 0x299999b0, 0x5a2d2d77, 0x1e0f0f11, 0x7bb0b0cb, 0xa85454fc, 0x6dbbbbd6, 0x2c16163a };
bit32 table1[256] = { 0xa5c66363, 0x84f87c7c, 0x99ee7777, 0x8df67b7b, 0xdfff2f2, 0xbdd66b6b, 0xb1de6f6f, 0x5491c5c5, 0x50603030, 0x3020101, 0xa9ce6767, 0x7d562b2b, 0x19e7fefe, 0x62b5d7d7, 0xe64dabab, 0x9aec7676, 0x458fcaca, 0x9d1f8282, 0x4089c9c9, 0x87fa7d7d, 0x15effafa, 0xebb25959, 0xc98e4747, 0xbfbf0f0, 0xec41adad, 0x67b3d4d4, 0xfd5fa2a2, 0xea45afaf, 0xbf239c9c, 0xf753a4a4, 0x96e47272, 0x5b9bc0c0, 0xc275b7b7, 0x1ce1fdfd, 0xae3d9393, 0x6a4c2626, 0x5a6c3636, 0x417e3f3f, 0x2f5f7f7, 0x4f83cccc, 0x5c683434, 0xf451a5a5, 0x34d1e5e5, 0x8f9f1f1, 0x93e27171, 0x73abd8d8, 0x53623131, 0x3f2a1515, 0xc080404, 0x5295c7c7, 0x65462323, 0x5e9dc3c3, 0x28301818, 0xa1379696, 0xf0a0505, 0xb52f9a9a, 0x90e0707, 0x36241212, 0x9b1b8080, 0x3ddfe2e2, 0x26cdebeb, 0x694e2727, 0xcd7fb2b2, 0x9fea7575, 0x1b120909, 0x9e1d8383, 0x74582c2c, 0x2e341a1a, 0x2d361b1b, 0xb2dc6e6e, 0xeeb45a5a, 0xfb5ba0a0, 0xf6a45252, 0x4d763b3b, 0x61b7d6d6, 0xce7db3b3, 0x7b522929, 0x3edde3e3, 0x715e2f2f, 0x97138484, 0xf5a65353, 0x68b9d1d1, 0x0, 0x2cc1eded, 0x60402020, 0x1fe3fcfc, 0xc879b1b1, 0xedb65b5b, 0xbed46a6a, 0x468dcbcb, 0xd967bebe, 0x4b723939, 0xde944a4a, 0xd4984c4c, 0xe8b05858, 0x4a85cfcf, 0x6bbbd0d0, 0x2ac5efef, 0xe54faaaa, 0x16edfbfb, 0xc5864343, 0xd79a4d4d, 0x55663333, 0x94118585, 0xcf8a4545, 0x10e9f9f9, 0x6040202, 0x81fe7f7f, 0xf0a05050, 0x44783c3c, 0xba259f9f, 0xe34ba8a8, 0xf3a25151, 0xfe5da3a3, 0xc0804040, 0x8a058f8f, 0xad3f9292, 0xbc219d9d, 0x48703838, 0x4f1f5f5, 0xdf63bcbc, 0xc177b6b6, 0x75afdada, 0x63422121, 0x30201010, 0x1ae5ffff, 0xefdf3f3, 0x6dbfd2d2, 0x4c81cdcd, 0x14180c0c, 0x35261313, 0x2fc3ecec, 0xe1be5f5f, 0xa2359797, 0xcc884444, 0x392e1717, 0x5793c4c4, 0xf255a7a7, 0x82fc7e7e, 0x477a3d3d, 0xacc86464, 0xe7ba5d5d, 0x2b321919, 0x95e67373, 0xa0c06060, 0x98198181, 0xd19e4f4f, 0x7fa3dcdc, 0x66442222, 0x7e542a2a, 0xab3b9090, 0x830b8888, 0xca8c4646, 0x29c7eeee, 0xd36bb8b8, 0x3c281414, 0x79a7dede, 0xe2bc5e5e, 0x1d160b0b, 0x76addbdb, 0x3bdbe0e0, 0x56643232, 0x4e743a3a, 0x1e140a0a, 0xdb924949, 0xa0c0606, 0x6c482424, 0xe4b85c5c, 0x5d9fc2c2, 0x6ebdd3d3, 0xef43acac, 0xa6c46262, 0xa8399191, 0xa4319595, 0x37d3e4e4, 0x8bf27979, 0x32d5e7e7, 0x438bc8c8, 0x596e3737, 0xb7da6d6d, 0x8c018d8d, 0x64b1d5d5, 0xd29c4e4e, 0xe049a9a9, 0xb4d86c6c, 0xfaac5656, 0x7f3f4f4, 0x25cfeaea, 0xafca6565, 0x8ef47a7a, 0xe947aeae, 0x18100808, 0xd56fbaba, 0x88f07878, 0x6f4a2525, 0x725c2e2e, 0x24381c1c, 0xf157a6a6, 0xc773b4b4, 0x5197c6c6, 0x23cbe8e8, 0x7ca1dddd, 0x9ce87474, 0x213e1f1f, 0xdd964b4b, 0xdc61bdbd, 0x860d8b8b, 0x850f8a8a, 0x90e07070, 0x427c3e3e, 0xc471b5b5, 0xaacc6666, 0xd8904848, 0x5060303, 0x1f7f6f6, 0x121c0e0e, 0xa3c26161, 0x5f6a3535, 0xf9ae5757, 0xd069b9b9, 0x91178686, 0x5899c1c1, 0x273a1d1d, 0xb9279e9e, 0x38d9e1e1, 0x13ebf8f8, 0xb32b9898, 0x33221111, 0xbbd26969, 0x70a9d9d9, 0x89078e8e, 0xa7339494, 0xb62d9b9b, 0x223c1e1e, 0x92158787, 0x20c9e9e9, 0x4987cece, 0xffaa5555, 0x78502828, 0x7aa5dfdf, 0x8f038c8c, 0xf859a1a1, 0x80098989, 0x171a0d0d, 0xda65bfbf, 0x31d7e6e6, 0xc6844242, 0xb8d06868, 0xc3824141, 0xb0299999, 0x775a2d2d, 0x111e0f0f, 0xcb7bb0b0, 0xfca85454, 0xd66dbbbb, 0x3a2c1616 };
bit32 table2[256] = { 0x63a5c663, 0x7c84f87c, 0x7799ee77, 0x7b8df67b, 0xf20dfff2, 0x6bbdd66b, 0x6fb1de6f, 0xc55491c5, 0x30506030, 0x1030201, 0x67a9ce67, 0x2b7d562b, 0xfe19e7fe, 0xd762b5d7, 0xabe64dab, 0x769aec76, 0xca458fca, 0x829d1f82, 0xc94089c9, 0x7d87fa7d, 0xfa15effa, 0x59ebb259, 0x47c98e47, 0xf00bfbf0, 0xadec41ad, 0xd467b3d4, 0xa2fd5fa2, 0xafea45af, 0x9cbf239c, 0xa4f753a4, 0x7296e472, 0xc05b9bc0, 0xb7c275b7, 0xfd1ce1fd, 0x93ae3d93, 0x266a4c26, 0x365a6c36, 0x3f417e3f, 0xf702f5f7, 0xcc4f83cc, 0x345c6834, 0xa5f451a5, 0xe534d1e5, 0xf108f9f1, 0x7193e271, 0xd873abd8, 0x31536231, 0x153f2a15, 0x40c0804, 0xc75295c7, 0x23654623, 0xc35e9dc3, 0x18283018, 0x96a13796, 0x50f0a05, 0x9ab52f9a, 0x7090e07, 0x12362412, 0x809b1b80, 0xe23ddfe2, 0xeb26cdeb, 0x27694e27, 0xb2cd7fb2, 0x759fea75, 0x91b1209, 0x839e1d83, 0x2c74582c, 0x1a2e341a, 0x1b2d361b, 0x6eb2dc6e, 0x5aeeb45a, 0xa0fb5ba0, 0x52f6a452, 0x3b4d763b, 0xd661b7d6, 0xb3ce7db3, 0x297b5229, 0xe33edde3, 0x2f715e2f, 0x84971384, 0x53f5a653, 0xd168b9d1, 0x0, 0xed2cc1ed, 0x20604020, 0xfc1fe3fc, 0xb1c879b1, 0x5bedb65b, 0x6abed46a, 0xcb468dcb, 0xbed967be, 0x394b7239, 0x4ade944a, 0x4cd4984c, 0x58e8b058, 0xcf4a85cf, 0xd06bbbd0, 0xef2ac5ef, 0xaae54faa, 0xfb16edfb, 0x43c58643, 0x4dd79a4d, 0x33556633, 0x85941185, 0x45cf8a45, 0xf910e9f9, 0x2060402, 0x7f81fe7f, 0x50f0a050, 0x3c44783c, 0x9fba259f, 0xa8e34ba8, 0x51f3a251, 0xa3fe5da3, 0x40c08040, 0x8f8a058f, 0x92ad3f92, 0x9dbc219d, 0x38487038, 0xf504f1f5, 0xbcdf63bc, 0xb6c177b6, 0xda75afda, 0x21634221, 0x10302010, 0xff1ae5ff, 0xf30efdf3, 0xd26dbfd2, 0xcd4c81cd, 0xc14180c, 0x13352613, 0xec2fc3ec, 0x5fe1be5f, 0x97a23597, 0x44cc8844, 0x17392e17, 0xc45793c4, 0xa7f255a7, 0x7e82fc7e, 0x3d477a3d, 0x64acc864, 0x5de7ba5d, 0x192b3219, 0x7395e673, 0x60a0c060, 0x81981981, 0x4fd19e4f, 0xdc7fa3dc, 0x22664422, 0x2a7e542a, 0x90ab3b90, 0x88830b88, 0x46ca8c46, 0xee29c7ee, 0xb8d36bb8, 0x143c2814, 0xde79a7de, 0x5ee2bc5e, 0xb1d160b, 0xdb76addb, 0xe03bdbe0, 0x32566432, 0x3a4e743a, 0xa1e140a, 0x49db9249, 0x60a0c06, 0x246c4824, 0x5ce4b85c, 0xc25d9fc2, 0xd36ebdd3, 0xacef43ac, 0x62a6c462, 0x91a83991, 0x95a43195, 0xe437d3e4, 0x798bf279, 0xe732d5e7, 0xc8438bc8, 0x37596e37, 0x6db7da6d, 0x8d8c018d, 0xd564b1d5, 0x4ed29c4e, 0xa9e049a9, 0x6cb4d86c, 0x56faac56, 0xf407f3f4, 0xea25cfea, 0x65afca65, 0x7a8ef47a, 0xaee947ae, 0x8181008, 0xbad56fba, 0x7888f078, 0x256f4a25, 0x2e725c2e, 0x1c24381c, 0xa6f157a6, 0xb4c773b4, 0xc65197c6, 0xe823cbe8, 0xdd7ca1dd, 0x749ce874, 0x1f213e1f, 0x4bdd964b, 0xbddc61bd, 0x8b860d8b, 0x8a850f8a, 0x7090e070, 0x3e427c3e, 0xb5c471b5, 0x66aacc66, 0x48d89048, 0x3050603, 0xf601f7f6, 0xe121c0e, 0x61a3c261, 0x355f6a35, 0x57f9ae57, 0xb9d069b9, 0x86911786, 0xc15899c1, 0x1d273a1d, 0x9eb9279e, 0xe138d9e1, 0xf813ebf8, 0x98b32b98, 0x11332211, 0x69bbd269, 0xd970a9d9, 0x8e89078e, 0x94a73394, 0x9bb62d9b, 0x1e223c1e, 0x87921587, 0xe920c9e9, 0xce4987ce, 0x55ffaa55, 0x28785028, 0xdf7aa5df, 0x8c8f038c, 0xa1f859a1, 0x89800989, 0xd171a0d, 0xbfda65bf, 0xe631d7e6, 0x42c68442, 0x68b8d068, 0x41c38241, 0x99b02999, 0x2d775a2d, 0xf111e0f, 0xb0cb7bb0, 0x54fca854, 0xbbd66dbb, 0x163a2c16 };
bit32 table3[256] = { 0x6363a5c6, 0x7c7c84f8, 0x777799ee, 0x7b7b8df6, 0xf2f20dff, 0x6b6bbdd6, 0x6f6fb1de, 0xc5c55491, 0x30305060, 0x1010302, 0x6767a9ce, 0x2b2b7d56, 0xfefe19e7, 0xd7d762b5, 0xababe64d, 0x76769aec, 0xcaca458f, 0x82829d1f, 0xc9c94089, 0x7d7d87fa, 0xfafa15ef, 0x5959ebb2, 0x4747c98e, 0xf0f00bfb, 0xadadec41, 0xd4d467b3, 0xa2a2fd5f, 0xafafea45, 0x9c9cbf23, 0xa4a4f753, 0x727296e4, 0xc0c05b9b, 0xb7b7c275, 0xfdfd1ce1, 0x9393ae3d, 0x26266a4c, 0x36365a6c, 0x3f3f417e, 0xf7f702f5, 0xcccc4f83, 0x34345c68, 0xa5a5f451, 0xe5e534d1, 0xf1f108f9, 0x717193e2, 0xd8d873ab, 0x31315362, 0x15153f2a, 0x4040c08, 0xc7c75295, 0x23236546, 0xc3c35e9d, 0x18182830, 0x9696a137, 0x5050f0a, 0x9a9ab52f, 0x707090e, 0x12123624, 0x80809b1b, 0xe2e23ddf, 0xebeb26cd, 0x2727694e, 0xb2b2cd7f, 0x75759fea, 0x9091b12, 0x83839e1d, 0x2c2c7458, 0x1a1a2e34, 0x1b1b2d36, 0x6e6eb2dc, 0x5a5aeeb4, 0xa0a0fb5b, 0x5252f6a4, 0x3b3b4d76, 0xd6d661b7, 0xb3b3ce7d, 0x29297b52, 0xe3e33edd, 0x2f2f715e, 0x84849713, 0x5353f5a6, 0xd1d168b9, 0x0, 0xeded2cc1, 0x20206040, 0xfcfc1fe3, 0xb1b1c879, 0x5b5bedb6, 0x6a6abed4, 0xcbcb468d, 0xbebed967, 0x39394b72, 0x4a4ade94, 0x4c4cd498, 0x5858e8b0, 0xcfcf4a85, 0xd0d06bbb, 0xefef2ac5, 0xaaaae54f, 0xfbfb16ed, 0x4343c586, 0x4d4dd79a, 0x33335566, 0x85859411, 0x4545cf8a, 0xf9f910e9, 0x2020604, 0x7f7f81fe, 0x5050f0a0, 0x3c3c4478, 0x9f9fba25, 0xa8a8e34b, 0x5151f3a2, 0xa3a3fe5d, 0x4040c080, 0x8f8f8a05, 0x9292ad3f, 0x9d9dbc21, 0x38384870, 0xf5f504f1, 0xbcbcdf63, 0xb6b6c177, 0xdada75af, 0x21216342, 0x10103020, 0xffff1ae5, 0xf3f30efd, 0xd2d26dbf, 0xcdcd4c81, 0xc0c1418, 0x13133526, 0xecec2fc3, 0x5f5fe1be, 0x9797a235, 0x4444cc88, 0x1717392e, 0xc4c45793, 0xa7a7f255, 0x7e7e82fc, 0x3d3d477a, 0x6464acc8, 0x5d5de7ba, 0x19192b32, 0x737395e6, 0x6060a0c0, 0x81819819, 0x4f4fd19e, 0xdcdc7fa3, 0x22226644, 0x2a2a7e54, 0x9090ab3b, 0x8888830b, 0x4646ca8c, 0xeeee29c7, 0xb8b8d36b, 0x14143c28, 0xdede79a7, 0x5e5ee2bc, 0xb0b1d16, 0xdbdb76ad, 0xe0e03bdb, 0x32325664, 0x3a3a4e74, 0xa0a1e14, 0x4949db92, 0x6060a0c, 0x24246c48, 0x5c5ce4b8, 0xc2c25d9f, 0xd3d36ebd, 0xacacef43, 0x6262a6c4, 0x9191a839, 0x9595a431, 0xe4e437d3, 0x79798bf2, 0xe7e732d5, 0xc8c8438b, 0x3737596e, 0x6d6db7da, 0x8d8d8c01, 0xd5d564b1, 0x4e4ed29c, 0xa9a9e049, 0x6c6cb4d8, 0x5656faac, 0xf4f407f3, 0xeaea25cf, 0x6565afca, 0x7a7a8ef4, 0xaeaee947, 0x8081810, 0xbabad56f, 0x787888f0, 0x25256f4a, 0x2e2e725c, 0x1c1c2438, 0xa6a6f157, 0xb4b4c773, 0xc6c65197, 0xe8e823cb, 0xdddd7ca1, 0x74749ce8, 0x1f1f213e, 0x4b4bdd96, 0xbdbddc61, 0x8b8b860d, 0x8a8a850f, 0x707090e0, 0x3e3e427c, 0xb5b5c471, 0x6666aacc, 0x4848d890, 0x3030506, 0xf6f601f7, 0xe0e121c, 0x6161a3c2, 0x35355f6a, 0x5757f9ae, 0xb9b9d069, 0x86869117, 0xc1c15899, 0x1d1d273a, 0x9e9eb927, 0xe1e138d9, 0xf8f813eb, 0x9898b32b, 0x11113322, 0x6969bbd2, 0xd9d970a9, 0x8e8e8907, 0x9494a733, 0x9b9bb62d, 0x1e1e223c, 0x87879215, 0xe9e920c9, 0xcece4987, 0x5555ffaa, 0x28287850, 0xdfdf7aa5, 0x8c8c8f03, 0xa1a1f859, 0x89898009, 0xd0d171a, 0xbfbfda65, 0xe6e631d7, 0x4242c684, 0x6868b8d0, 0x4141c382, 0x9999b029, 0x2d2d775a, 0xf0f111e, 0xb0b0cb7b, 0x5454fca8, 0xbbbbd66d, 0x16163a2c };
bit32 SAES[256] = { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
bit32 RconAES[256] = {	0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,	0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,	0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,	0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,	0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,	0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,	0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,	0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,	0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,	0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,	0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,	0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,	0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,	0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,	0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,	0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d };
bit32 b[4] = { 0, 0, 0, 0 };
/// END of REAL AES
bit64 S[16] = {0x6, 0xb, 0x5, 0x4, 0x2, 0xe, 0x7, 0xa, 0x9, 0xd, 0xf, 0xc, 0x3, 0x1, 0x0, 0x8};
bit64 Sinv[16] = { 0xe, 0xd, 0x4, 0xc, 0x3, 0x2, 0x0, 0x6, 0xf, 0x8, 0x7, 0x1, 0xb, 0x9, 0x5, 0xa };
//bit64 SAES[256] = { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
bit64 SAESinv[256] = { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
//bit32 Rcon[16] = { 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a };
bit32 Rcon[16] = { 0x8d, 0x01, 0x02, 0x04, 0x08, 0x13, 0x25, 0x47, 0x89, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a };
bit64 rkey[20] = { 0 };
bit64 alpha[28] = { 0 }, alpha_prime[28] = { 0 }, n[28] = { 100 }, one = 1;
bit8 antidiagonal1test[65536] = { 0 };
bit8 antidiagonal12[16384][65536] = { 0 };
//bit8 antidiagonal12b[16384][65536] = { 0 };
bit64 antidiagonal1[65536] = { 0 };
bit64 antidiagonal2[65536] = { 0 };
bit64 antidiagonal3[65536] = { 0 };
bit64 antidiagonal4[65536] = { 0 };
// New tests
bit64 byte1[16][16] = { 0 }, hits1[16] = { 0 };
bit64 byte2[120][256] = { 0 }, hits2[120] = { 0 };
bit64 byte3[560][4096] = { 0 }, hits3[560] = { 0 };
bit64 byte4[1820][65536] = { 0 }, hits4[1820] = { 0 };
//bit8 antidiagonal12[4294967296] = { 0 };
//bit8 antidiagonal34[4294967296] = { 0 };
bit64 count[100] = { 0 };
bit64 hits[4] = { 0 };
void key_schedule(bit64 key) {
	bit64 o[4], temp;
	rkey[0] = key;
	for (int i = 1; i < 16; i++) {
		o[0] = (key >> 48) & 0xffff;
		o[1] = (key >> 32) & 0xffff;
		o[2] = (key >> 16) & 0xffff;
		o[3] = (key >> 0) & 0xffff;
		temp = o[3];
		temp = ROTL16(temp, 4);
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ ((S[(temp >> 12) & 0xf] ^ (Rcon[i] & 0xf)) << 12);
		if (temp>0xffff) printf("ERROR\n");
		o[0] ^= temp;
		o[1] ^= o[0];
		o[2] ^= o[1];
		o[3] ^= o[2];
		key = (o[0] << 48) ^ (o[1] << 32) ^ (o[2] << 16) ^ (o[3] << 0);
		rkey[i] = key;
	}
}
void key_schedule_inverse(bit64 key, int round) {
	bit64 o[4], temp;
	rkey[0] = key;
	for (int i = 1; i < round; i++) {
		o[0] = (key >> 48) & 0xffff;
		o[1] = (key >> 32) & 0xffff;
		o[2] = (key >> 16) & 0xffff;
		o[3] = (key >> 0) & 0xffff;
		o[3] ^= o[2];
		o[2] ^= o[1];
		o[1] ^= o[0];
		temp = o[3];
		temp = ROTL16(temp, 4);
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ ((S[(temp >> 12) & 0xf] ^ (Rcon[round-i] & 0xf)) << 12);
		if (temp>0xffff) printf("ERROR\n");
		o[0] ^= temp;
		key = (o[0] << 48) ^ (o[1] << 32) ^ (o[2] << 16) ^ (o[3] << 0);
		rkey[i] = key;
	}
}
void key_schedule128(bit64 key[2]) {
	bit64 o[4], temp;
	rkey[0] = key[0];
	rkey[1] = key[1];
	for (int i = 2; i < 14; i=i+2) {
		o[0] = (key[0] >> 48) & 0xffff;
		o[1] = (key[0] >> 32) & 0xffff;
		o[2] = (key[0] >> 16) & 0xffff;
		o[3] = (key[0] >> 0) & 0xffff;
		o[4] = (key[1] >> 48) & 0xffff;
		o[5] = (key[1] >> 32) & 0xffff;
		o[6] = (key[1] >> 16) & 0xffff;
		o[7] = (key[1] >> 0) & 0xffff;
		temp = o[7];
		temp = ROTL16(temp, 4);
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ ((S[(temp >> 12) & 0xf] ^ (Rcon[i/2] & 0xf)) << 12);
		if (temp>0xffff) printf("ERROR\n");
		o[0] ^= temp;
		o[1] ^= o[0];
		o[2] ^= o[1];
		o[3] ^= o[2];
		temp = o[3];
//		temp = ROTL16(temp, 4);
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ (S[(temp >> 12) & 0xf] << 12);
		if (temp>0xffff) printf("ERROR\n");
		o[4] ^= temp;
		o[5] ^= o[4];
		o[6] ^= o[5];
		o[7] ^= o[6];
		key[0] = (o[0] << 48) ^ (o[1] << 32) ^ (o[2] << 16) ^ (o[3] << 0);
		key[1] = (o[4] << 48) ^ (o[5] << 32) ^ (o[6] << 16) ^ (o[7] << 0);
		rkey[i] = key[0];
		rkey[i+1] = key[1];
	}
}
void key_schedule128_inverse(bit64 key[2], int round) {
	bit64 o[4], temp;
	rkey[0] = key[0];
	rkey[1] = key[1];
	for (int i = 2; i < round; i = i + 2) {
		o[0] = (key[0] >> 48) & 0xffff;
		o[1] = (key[0] >> 32) & 0xffff;
		o[2] = (key[0] >> 16) & 0xffff;
		o[3] = (key[0] >> 0) & 0xffff;
		o[4] = (key[1] >> 48) & 0xffff;
		o[5] = (key[1] >> 32) & 0xffff;
		o[6] = (key[1] >> 16) & 0xffff;
		o[7] = (key[1] >> 0) & 0xffff;
		o[7] ^= o[6];
		o[6] ^= o[5];
		o[5] ^= o[4];
		temp = o[3];
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ (S[(temp >> 12) & 0xf] << 12);
		o[4] ^= temp;
		if (temp>0xffff) printf("ERROR\n");
		o[3] ^= o[2];
		o[2] ^= o[1];
		o[1] ^= o[0];
		temp = o[7];
		temp = ROTL16(temp, 4);
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ ((S[(temp >> 12) & 0xf] ^ (Rcon[(round-i)/2] & 0xf)) << 12);
		o[0] ^= temp;
		if (temp>0xffff) printf("ERROR\n");
		key[0] = (o[0] << 48) ^ (o[1] << 32) ^ (o[2] << 16) ^ (o[3] << 0);
		key[1] = (o[4] << 48) ^ (o[5] << 32) ^ (o[6] << 16) ^ (o[7] << 0);
		rkey[i] = key[0];
		rkey[i + 1] = key[1];
	}
}
void key_schedule128_test(bit64 key[2]) {
	key_schedule128(key);
	for (int i = 0; i < 20; i=i+2) {
		printf("rkey %02d, %02d: %016I64x %016I64x\n", i, i+1, rkey[i], rkey[i+1]);
	}
}
void AES192_experiment() {
	//bit64 a = 0x12, b = 0x34, c = 0x56, d = 0x78, e = 0x9a, f = 0xbc, g = 0xde, h = 0xf8;
	//bit64 a = 0x00, b = 0x00, c = 0x00, d = 0x00, e = 0x9a, f = 0xbc, g = 0xde, h = 0xf8;
	bit64 a = 0x12, b = 0x34, c = 0x56, d = 0x78, e = 0x0, f = 0x0, g = 0x0, h = 0x0;
	printf("d^h: %I64x %I64x\n", SAESinv[d^h], e^SAESinv[d^h]);
	printf("a^e: %I64x %I64x %I64x %I64x\n", SAESinv[a^e], f^SAESinv[a^e], SAESinv[a^e ^ 0x01], f^SAESinv[a^e ^ 0x01]);
	printf("b^f: %I64x %I64x\n", SAESinv[b^f], g^SAESinv[b^f]);
	printf("c^g: %I64x %I64x\n", SAESinv[c^g], h^SAESinv[c^g]);
}
bit64 matrix_multiplication(bit64 input) {
	bit64 i[4], o[4], output;
	i[0] = (input >> 12) & 0xf;
	i[1] = (input >> 8) & 0xf;
	i[2] = (input >> 4) & 0xf;
	i[3] = (input >> 0) & 0xf;
	// first row
	o[0] = i[0] << 1; if (o[0] >> 4) o[0] = (o[0] & 0xf) ^ 0x3;
	o[1] = i[1] << 1; if (o[1] >> 4) o[1] = (o[1] & 0xf) ^ 0x3; o[1] ^= i[1];
	o[2] = i[2];
	o[3] = i[3];
	o[0] = o[0] ^ o[1] ^ o[2] ^ o[3];
	if (o[0] > 0xf) printf("Error\n");
	output = o[0] << 12;
	// second row
	o[0] = i[0];
	o[1] = i[1] << 1; if (o[1] >> 4) o[1] = (o[1] & 0xf) ^ 0x3;
	o[2] = i[2] << 1; if (o[2] >> 4) o[2] = (o[2] & 0xf) ^ 0x3; o[2] ^= i[2];
	o[3] = i[3];
	o[0] = o[0] ^ o[1] ^ o[2] ^ o[3];
	if (o[0] > 0xf) printf("Error\n");
	output ^= (o[0] << 8);
	// third row
	o[0] = i[0];
	o[1] = i[1];
	o[2] = i[2] << 1; if (o[2] >> 4) o[2] = (o[2] & 0xf) ^ 0x3;
	o[3] = i[3] << 1; if (o[3] >> 4) o[3] = (o[3] & 0xf) ^ 0x3; o[3] ^= i[3];
	o[0] = o[0] ^ o[1] ^ o[2] ^ o[3];
	if (o[0] > 0xf) printf("Error\n");
	output ^= (o[0] << 4);
	// fourth row
	o[0] = i[0] << 1; if (o[0] >> 4) o[0] = (o[0] & 0xf) ^ 0x3; o[0] ^= i[0];
	o[1] = i[1];
	o[2] = i[2];
	o[3] = i[3] << 1; if (o[3] >> 4) o[3] = (o[3] & 0xf) ^ 0x3;
	o[0] = o[0] ^ o[1] ^ o[2] ^ o[3];
	if (o[0] > 0xf) printf("Error\n");
	output ^= o[0];
	if (output > 0xffff) printf("Error\n");
	return output;
}
bit64 encrypt(bit64 plaintext, int round) {
	bit64 state=plaintext, temp[4];
//	state = (plaintext & 0xf0000f0000f0000f) ^ ((plaintext & 0x0f0000f0000f0000) >> 12) ^ ((plaintext & 0x00f0000f00000000) >> 24) ^ ((plaintext & 0x000f000000000000) >> 36) ^ ((plaintext & 0x0000f0000f0000f0) << 12) ^ ((plaintext & 0x00000000f0000f00) << 24) ^ ((plaintext & 0x000000000000f000) << 36);
	for (int i = 0; i < round-1; i++) {
		// key addition
//		rkey[i] = (rkey[i] & 0xf0000f0000f0000f) ^ ((rkey[i] & 0x0f0000f0000f0000) >> 12) ^ ((rkey[i] & 0x00f0000f00000000) >> 24) ^ ((rkey[i] & 0x000f000000000000) >> 36) ^ ((rkey[i] & 0x0000f0000f0000f0) << 12) ^ ((rkey[i] & 0x00000000f0000f00) << 24) ^ ((rkey[i] & 0x000000000000f000) << 36);
		state ^= rkey[i];
		// S-box Layer
		state = S[state & 0xf] ^ (S[(state >> 4) & 0xf] << 4) ^ (S[(state >> 8) & 0xf] << 8) ^ (S[(state >> 12) & 0xf] << 12) ^ (S[(state >> 16) & 0xf] << 16) ^ (S[(state >> 20) & 0xf] << 20) ^ (S[(state >> 24) & 0xf] << 24) ^ (S[(state >> 28) & 0xf] << 28) ^ (S[(state >> 32) & 0xf] << 32) ^ (S[(state >> 36) & 0xf] << 36) ^ (S[(state >> 40) & 0xf] << 40) ^ (S[(state >> 44) & 0xf] << 44) ^ (S[(state >> 48) & 0xf] << 48) ^ (S[(state >> 52) & 0xf] << 52) ^ (S[(state >> 56) & 0xf] << 56) ^ (S[(state >> 60) & 0xf] << 60);
//		printf("%016I64x (S-box)\n", state);
		// Shift row
		temp[0] = (state & 0xf000f000f000f000);
		temp[1] = (state & 0x0f000f000f000f00);
		temp[2] = (state & 0x00f000f000f000f0);
		temp[3] = (state & 0x000f000f000f000f);
		temp[1] = ROTL(temp[1], 16);
		temp[2] = ROTL(temp[2], 32);
		temp[3] = ROTL(temp[3], 48);
		state = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
/*		temp[0] = (state >> 48) & 0xffff;
		temp[1] = (state >> 32) & 0xffff; temp[1] = ROTL16(temp[1], 4);
		temp[2] = (state >> 16) & 0xffff; temp[2] = ROTL16(temp[2], 8);
		temp[3] = (state >> 0) & 0xffff; temp[3] = ROTL16(temp[3], 12);
		state = (temp[0] << 48) ^ (temp[1] << 32) ^ (temp[2] << 16) ^ (temp[3] << 0);*/
//		printf("%016I64x (Shift Row)\n", state);
		// Matrix multiplication
/*		temp[0] = (((state >> 60) & 0xf) << 12) ^ (((state >> 44) & 0xf) << 8) ^ (((state >> 28) & 0xf) << 4) ^ (((state >> 12) & 0xf) << 0);
		temp[1] = (((state >> 56) & 0xf) << 12) ^ (((state >> 40) & 0xf) << 8) ^ (((state >> 24) & 0xf) << 4) ^ (((state >> 8) & 0xf) << 0);
		temp[2] = (((state >> 52) & 0xf) << 12) ^ (((state >> 36) & 0xf) << 8) ^ (((state >> 20) & 0xf) << 4) ^ (((state >> 4) & 0xf) << 0);
		temp[3] = (((state >> 48) & 0xf) << 12) ^ (((state >> 32) & 0xf) << 8) ^ (((state >> 16) & 0xf) << 4) ^ (((state >> 0) & 0xf) << 0);
		temp[0] = matrix_multiplication(temp[0]);
		temp[1] = matrix_multiplication(temp[1]);
		temp[2] = matrix_multiplication(temp[2]);
		temp[3] = matrix_multiplication(temp[3]);
		state = ((temp[0] & 0xf000) << 48) ^ ((temp[0] & 0x0f00) << 36) ^ ((temp[0] & 0x00f0) << 24) ^ ((temp[0] & 0xf) << 12);
		state ^= ((temp[1] & 0xf000) << 44) ^ ((temp[1] & 0x0f00) << 32) ^ ((temp[1] & 0x00f0) << 20) ^ ((temp[1] & 0xf) << 8);
		state ^= ((temp[2] & 0xf000) << 40) ^ ((temp[2] & 0x0f00) << 28) ^ ((temp[2] & 0x00f0) << 16) ^ ((temp[2] & 0xf) << 4);
		state ^= ((temp[3] & 0xf000) << 36) ^ ((temp[3] & 0x0f00) << 24) ^ ((temp[3] & 0x00f0) << 12) ^ ((temp[3] & 0xf) << 0);*/
		temp[0] = (state >> 48) & 0xffff;
		temp[1] = (state >> 32) & 0xffff;
		temp[2] = (state >> 16) & 0xffff;
		temp[3] = (state >> 0) & 0xffff;
/*		temp[0] = matrix_multiplication(temp[0]);
		temp[1] = matrix_multiplication(temp[1]);
		temp[2] = matrix_multiplication(temp[2]);
		temp[3] = matrix_multiplication(temp[3]);*/
		temp[0] = mc[temp[0]];
		temp[1] = mc[temp[1]];
		temp[2] = mc[temp[2]];
		temp[3] = mc[temp[3]];
		state = (temp[0] << 48) ^ (temp[1] << 32) ^ (temp[2] << 16) ^ (temp[3] << 0);
//		printf("%016I64x (Matrix multiplication)\n", state);
	}
	// Last round without Mixcolumn
	// key addition
//	rkey[round - 1] = (rkey[round - 1] & 0xf0000f0000f0000f) ^ ((rkey[round - 1] & 0x0f0000f0000f0000) >> 12) ^ ((rkey[round - 1] & 0x00f0000f00000000) >> 24) ^ ((rkey[round - 1] & 0x000f000000000000) >> 36) ^ ((rkey[round - 1] & 0x0000f0000f0000f0) << 12) ^ ((rkey[round - 1] & 0x00000000f0000f00) << 24) ^ ((rkey[round - 1] & 0x000000000000f000) << 36);
	state ^= rkey[round-1];
	// S-box Layer
	state = S[state & 0xf] ^ (S[(state >> 4) & 0xf] << 4) ^ (S[(state >> 8) & 0xf] << 8) ^ (S[(state >> 12) & 0xf] << 12) ^ (S[(state >> 16) & 0xf] << 16) ^ (S[(state >> 20) & 0xf] << 20) ^ (S[(state >> 24) & 0xf] << 24) ^ (S[(state >> 28) & 0xf] << 28) ^ (S[(state >> 32) & 0xf] << 32) ^ (S[(state >> 36) & 0xf] << 36) ^ (S[(state >> 40) & 0xf] << 40) ^ (S[(state >> 44) & 0xf] << 44) ^ (S[(state >> 48) & 0xf] << 48) ^ (S[(state >> 52) & 0xf] << 52) ^ (S[(state >> 56) & 0xf] << 56) ^ (S[(state >> 60) & 0xf] << 60);
	//		printf("%016I64x (S-box)\n", state);
	// Shift row
	temp[0] = (state & 0xf000f000f000f000);
	temp[1] = (state & 0x0f000f000f000f00);
	temp[2] = (state & 0x00f000f000f000f0);
	temp[3] = (state & 0x000f000f000f000f);
	temp[1] = ROTL(temp[1], 16);
	temp[2] = ROTL(temp[2], 32);
	temp[3] = ROTL(temp[3], 48);
	state = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
/*	temp[0] = (state >> 48) & 0xffff;
	temp[1] = (state >> 32) & 0xffff; temp[1] = ROTL16(temp[1], 4);
	temp[2] = (state >> 16) & 0xffff; temp[2] = ROTL16(temp[2], 8);
	temp[3] = (state >> 0) & 0xffff; temp[3] = ROTL16(temp[3], 12);
	state = (temp[0] << 48) ^ (temp[1] << 32) ^ (temp[2] << 16) ^ (temp[3] << 0);*/
	// TEST for adding final mixcolumn //
/*	temp[0] = (state >> 48) & 0xffff;
	temp[1] = (state >> 32) & 0xffff;
	temp[2] = (state >> 16) & 0xffff;
	temp[3] = (state >> 0) & 0xffff;
	temp[0] = matrix_multiplication(temp[0]);
	temp[1] = matrix_multiplication(temp[1]);
	temp[2] = matrix_multiplication(temp[2]);
	temp[3] = matrix_multiplication(temp[3]);
	state = (temp[0] << 48) ^ (temp[1] << 32) ^ (temp[2] << 16) ^ (temp[3] << 0);*/
	return state;

}
bit64 decrypt(bit64 plaintext, int round) {
	bit64 state = plaintext, temp[4];
	// FIRST AND LAST ROUND HAVE MIXCOLUMNS!!!!!!
	// key addition
	state ^= rkey[0];
	// Matrix multiplication
	temp[0] = (state >> 48) & 0xffff;
	temp[1] = (state >> 32) & 0xffff;
	temp[2] = (state >> 16) & 0xffff;
	temp[3] = (state >> 0) & 0xffff;
	temp[0] = imc[temp[0]];
	temp[1] = imc[temp[1]];
	temp[2] = imc[temp[2]];
	temp[3] = imc[temp[3]];
	state = (temp[0] << 48) ^ (temp[1] << 32) ^ (temp[2] << 16) ^ (temp[3] << 0);
	// Shift row
	temp[0] = (state & 0xf000f000f000f000);
	temp[1] = (state & 0x0f000f000f000f00);
	temp[2] = (state & 0x00f000f000f000f0);
	temp[3] = (state & 0x000f000f000f000f);
	temp[1] = ROTR(temp[1], 16);
	temp[2] = ROTR(temp[2], 32);
	temp[3] = ROTR(temp[3], 48);
	state = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
	// S-box Layer
	state = Sinv[state & 0xf] ^ (Sinv[(state >> 4) & 0xf] << 4) ^ (Sinv[(state >> 8) & 0xf] << 8) ^ (Sinv[(state >> 12) & 0xf] << 12) ^ (Sinv[(state >> 16) & 0xf] << 16) ^ (Sinv[(state >> 20) & 0xf] << 20) ^ (Sinv[(state >> 24) & 0xf] << 24) ^ (Sinv[(state >> 28) & 0xf] << 28) ^ (Sinv[(state >> 32) & 0xf] << 32) ^ (Sinv[(state >> 36) & 0xf] << 36) ^ (Sinv[(state >> 40) & 0xf] << 40) ^ (Sinv[(state >> 44) & 0xf] << 44) ^ (Sinv[(state >> 48) & 0xf] << 48) ^ (Sinv[(state >> 52) & 0xf] << 52) ^ (Sinv[(state >> 56) & 0xf] << 56) ^ (Sinv[(state >> 60) & 0xf] << 60);

	for (int i = 1; i < round; i++) {
		// key addition
		state ^= rkey[i];
		// Matrix multiplication
		temp[0] = (state >> 48) & 0xffff;
		temp[1] = (state >> 32) & 0xffff;
		temp[2] = (state >> 16) & 0xffff;
		temp[3] = (state >> 0) & 0xffff;
		temp[0] = imc[temp[0]];
		temp[1] = imc[temp[1]];
		temp[2] = imc[temp[2]];
		temp[3] = imc[temp[3]];
		state = (temp[0] << 48) ^ (temp[1] << 32) ^ (temp[2] << 16) ^ (temp[3] << 0);
		// Shift row
		temp[0] = (state & 0xf000f000f000f000);
		temp[1] = (state & 0x0f000f000f000f00);
		temp[2] = (state & 0x00f000f000f000f0);
		temp[3] = (state & 0x000f000f000f000f);
		temp[1] = ROTR(temp[1], 16);
		temp[2] = ROTR(temp[2], 32);
		temp[3] = ROTR(temp[3], 48);
		state = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
		// S-box Layer
		state = Sinv[state & 0xf] ^ (Sinv[(state >> 4) & 0xf] << 4) ^ (Sinv[(state >> 8) & 0xf] << 8) ^ (Sinv[(state >> 12) & 0xf] << 12) ^ (Sinv[(state >> 16) & 0xf] << 16) ^ (Sinv[(state >> 20) & 0xf] << 20) ^ (Sinv[(state >> 24) & 0xf] << 24) ^ (Sinv[(state >> 28) & 0xf] << 28) ^ (Sinv[(state >> 32) & 0xf] << 32) ^ (Sinv[(state >> 36) & 0xf] << 36) ^ (Sinv[(state >> 40) & 0xf] << 40) ^ (Sinv[(state >> 44) & 0xf] << 44) ^ (Sinv[(state >> 48) & 0xf] << 48) ^ (Sinv[(state >> 52) & 0xf] << 52) ^ (Sinv[(state >> 56) & 0xf] << 56) ^ (Sinv[(state >> 60) & 0xf] << 60);
	}
	return state;

}
bit64 encrypt_no_whitening(bit64 plaintext, int round) {
	bit64 state = plaintext, temp[4];
	for (int i = 0; i < round - 1; i++) {
		// S-box Layer
		state = S[state & 0xf] ^ (S[(state >> 4) & 0xf] << 4) ^ (S[(state >> 8) & 0xf] << 8) ^ (S[(state >> 12) & 0xf] << 12) ^ (S[(state >> 16) & 0xf] << 16) ^ (S[(state >> 20) & 0xf] << 20) ^ (S[(state >> 24) & 0xf] << 24) ^ (S[(state >> 28) & 0xf] << 28) ^ (S[(state >> 32) & 0xf] << 32) ^ (S[(state >> 36) & 0xf] << 36) ^ (S[(state >> 40) & 0xf] << 40) ^ (S[(state >> 44) & 0xf] << 44) ^ (S[(state >> 48) & 0xf] << 48) ^ (S[(state >> 52) & 0xf] << 52) ^ (S[(state >> 56) & 0xf] << 56) ^ (S[(state >> 60) & 0xf] << 60);
		// Shift row
		temp[0] = (state & 0xf000f000f000f000);
		temp[1] = (state & 0x0f000f000f000f00);
		temp[2] = (state & 0x00f000f000f000f0);
		temp[3] = (state & 0x000f000f000f000f);
		temp[1] = ROTL(temp[1], 16);
		temp[2] = ROTL(temp[2], 32);
		temp[3] = ROTL(temp[3], 48);
		state = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
		// Matrix multiplication
		temp[0] = (state >> 48) & 0xffff;
		temp[1] = (state >> 32) & 0xffff;
		temp[2] = (state >> 16) & 0xffff;
		temp[3] = (state >> 0) & 0xffff;
		temp[0] = matrix_multiplication(temp[0]);
		temp[1] = matrix_multiplication(temp[1]);
		temp[2] = matrix_multiplication(temp[2]);
		temp[3] = matrix_multiplication(temp[3]);
		state = (temp[0] << 48) ^ (temp[1] << 32) ^ (temp[2] << 16) ^ (temp[3] << 0);
		state ^= rkey[i];
	}
	// Last round without Mixcolumn
	// S-box Layer
	state = S[state & 0xf] ^ (S[(state >> 4) & 0xf] << 4) ^ (S[(state >> 8) & 0xf] << 8) ^ (S[(state >> 12) & 0xf] << 12) ^ (S[(state >> 16) & 0xf] << 16) ^ (S[(state >> 20) & 0xf] << 20) ^ (S[(state >> 24) & 0xf] << 24) ^ (S[(state >> 28) & 0xf] << 28) ^ (S[(state >> 32) & 0xf] << 32) ^ (S[(state >> 36) & 0xf] << 36) ^ (S[(state >> 40) & 0xf] << 40) ^ (S[(state >> 44) & 0xf] << 44) ^ (S[(state >> 48) & 0xf] << 48) ^ (S[(state >> 52) & 0xf] << 52) ^ (S[(state >> 56) & 0xf] << 56) ^ (S[(state >> 60) & 0xf] << 60);
	// Shift row
	temp[0] = (state & 0xf000f000f000f000);
	temp[1] = (state & 0x0f000f000f000f00);
	temp[2] = (state & 0x00f000f000f000f0);
	temp[3] = (state & 0x000f000f000f000f);
	temp[1] = ROTL(temp[1], 16);
	temp[2] = ROTL(temp[2], 32);
	temp[3] = ROTL(temp[3], 48);
	state = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
	// key addition
	state ^= rkey[round - 1];
	return state;

}
void generate_ciphertexts(bit64 key, int round) {
	FILE *fp;
	bit64 plaintext = 0x0000000000000000, ciphertext;
	key_schedule(key);
	fopen_s(&fp, "ciphertext.txt", "w");
	//	for (int i = 0; i < 10; i++) printf("%d: %016I64x\n",i, rkey[i]);
	for (__int64 i = 0; i <= 0xffffffff; i++) {
		if ((i % 0x1000000) == 0) printf("Step: %I64x\n", i);
		ciphertext = encrypt(plaintext, round);
		fprintf(fp, "%016I64x\n", ciphertext);
		plaintext += 0x0000000100000001;
		//		printf("Encrypt(%016I64x) = %016I64x\n", plaintext, ciphertext);
	}
	//	printf("%016I64x (Plaintext)\n", plaintext);
	//	ciphertext = encrypt(plaintext, 1);
	//	printf("Encrypt(%016I64x) = %016I64x\n", plaintext, ciphertext);
	fclose(fp);

}
void on_the_fly(bit64 key, int round) {
	bit64 plaintext = 0x0000000000000000, ciphertext, temp[14] = { 0 }, input, xor1[4] = { 0 }, failcount=0;
//	bit8 *antidiagonal12;
//	antidiagonal12 = calloc(4294967296, sizeof(bit8));
	key_schedule(key);
	for (__int64 i = 0; i <= 0xffffffff; i++) {
		if ((i % 0x1000000) == 0) { printf("\b\b\b\b\b\b\b\b"); printf("Step: %I64x", i / 0x1000000); }
//		if ((i % 0x1000) == 0) printf("%d ",i);
		ciphertext = encrypt(plaintext, round);
		integral ^= ciphertext;
		input = ciphertext;
		temp[0] = ((input & 0xf000) >> 12) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf000000000000) >> 36);
		temp[1] = ((input & 0xf) >> 0) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf0000000000000) >> 40);
		temp[2] = ((input & 0xf0) >> 4) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf00000000000000) >> 44);
		temp[3] = ((input & 0xf00) >> 8) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal1[temp[0]]++;
		antidiagonal2[temp[1]]++;
		antidiagonal3[temp[2]]++;
		antidiagonal4[temp[3]]++;
/*		temp[0] = ((input & 0x3000) >> 12) ^ ((input & 0x3000000) >> 22) ^ ((input & 0x3000000000) >> 32) ^ ((input & 0x3000000000000) >> 42);
		temp[1] = ((input & 0x3) >> 0) ^ ((input & 0x30000000) >> 26) ^ ((input & 0x30000000000) >> 36) ^ ((input & 0x30000000000000) >> 46);
		temp[2] = ((input & 0x30) >> 4) ^ ((input & 0x30000) >> 14) ^ ((input & 0x300000000000) >> 40) ^ ((input & 0x300000000000000) >> 50);
		temp[3] = ((input & 0x300) >> 8) ^ ((input & 0x300000) >> 18) ^ ((input & 0x300000000) >> 28) ^ ((input & 0x3000000000000000) >> 54);*/
//		temp[4] = temp[0] | (temp[1] << 16);
//		antidiagonal1test[temp[0]]++;
//		if (temp[0]<16384) antidiagonal12[temp[0]][temp[1]]++;
//		else if (temp[0]<32768) antidiagonal12b[temp[0]-16384][temp[1]]++;
//		antidiagonal12[temp[4]]++;
/*		temp[5] = temp[0] | (temp[2] << 16);
		temp[6] = temp[0] | (temp[3] << 16);
		temp[7] = temp[1] | (temp[2] << 16);
		temp[8] = temp[1] | (temp[3] << 16);
		temp[9] = temp[2] | (temp[3] << 16);
		temp[10] = temp[0] | (temp[1] << 16) | (temp[2] << 32);
		temp[11] = temp[0] | (temp[1] << 16) | (temp[3] << 32);
		temp[12] = temp[0] | (temp[2] << 16) | (temp[3] << 32);
		temp[13] = temp[1] | (temp[2] << 16) | (temp[3] << 32);*/
/*		for (int j = 0; j < 14; j++) {
			alpha_prime[j] = (alpha[j] % (one << (2 * (temp[j]/4 + 1)))) / (one << (2 * (temp[j]/4)));
			alpha[j] = alpha[j] - (one << (2 * (temp[j]/4))) * alpha_prime[j];
			alpha_prime[j]++;
			alpha_prime[j] = alpha_prime[j] % 4;
			alpha[j] = alpha[j] + (one << (2 * (temp[j]/4))) * alpha_prime[j];
		}*/
/*		for (int j = 0; j < 14; j++) {
//			printf("Hello world\n");
//			printf("%I64d ", temp[j]);
			if (temp[j]) {
				alpha_prime[j] = (alpha[j] % (4 * (temp[j] + 1))) / (4 * temp[j]);
				alpha[j] = alpha[j] - (4 * temp[j]) * alpha_prime[j];
				alpha_prime[j]++;
				alpha_prime[j] = alpha_prime[j] % 4;
				alpha[j] = alpha[j] + (4 * temp[j]) * alpha_prime[j];
			}
			else {
				alpha_prime[j] = (alpha[j] % (4 * (temp[j] + 1)));
				alpha[j] = alpha[j] -  alpha_prime[j];
				alpha_prime[j]++;
				alpha_prime[j] = alpha_prime[j] % 4;
				alpha[j] = alpha[j] + alpha_prime[j];
			}
		}*/
		/*
		alpha_prime[1] = (alpha[1] % (one << (2 * (temp + 1)))) / (one << (2 * (temp)));
		alpha[1] = alpha[1] - (one << (2 * (temp))) * alpha_prime[1];
		alpha_prime[1]++;
		alpha_prime[1] = alpha_prime[1] % 4;
		alpha[1] = alpha[1] + (one << (2 * (temp))) * alpha_prime[1];
		
		
		alpha_prime[2] = (alpha[2] % (one << (2 * (temp + 1)))) / (one << (2 * (temp)));
		alpha[2] = alpha[2] - (one << (2 * (temp))) * alpha_prime[2];
		alpha_prime[2]++;
		alpha_prime[2] = alpha_prime[2] % 4;
		alpha[2] = alpha[2] + (one << (2 * (temp))) * alpha_prime[2];
		
		
		alpha_prime[3] = (alpha[3] % (one << (2 * (temp + 1)))) / (one << (2 * (temp)));
		alpha[3] = alpha[3] - (one << (2 * (temp))) * alpha_prime[3];
		alpha_prime[3]++;
		alpha_prime[3] = alpha_prime[3] % 4;
		alpha[3] = alpha[3] + (one << (2 * (temp))) * alpha_prime[3];*/
		plaintext += 0x0000000100000001;
	}
//	fopen_s(&f1, "anti1.txt", "w");
//	fopen_s(&f2, "anti2.txt", "w");
//	fopen_s(&f3, "anti3.txt", "w");
//	fopen_s(&f4, "anti4.txt", "w");
	for (int i = 0; i < 65536; i++) {
		if ((i % 2) == 0) {
			xor1[0] += antidiagonal1[i] * antidiagonal1[i + 1];
			xor1[1] += antidiagonal2[i] * antidiagonal2[i + 1];
			xor1[2] += antidiagonal3[i] * antidiagonal3[i + 1];
			xor1[3] += antidiagonal4[i] * antidiagonal4[i + 1];
		}
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
//		if ((antidiagonal1test[i] % 128)>0) printf("Exception found for antidiagonal 1: %d\n", i);
//		fprintf(f1,"%I64d,",antidiagonal1[i]);
//		fprintf(f2, "%I64d,", antidiagonal2[i]);
//		fprintf(f3, "%I64d,", antidiagonal3[i]);
//		fprintf(f4, "%I64d,", antidiagonal4[i]);
/*		alpha_prime[0] = alpha_prime[0] = (alpha[0] % (one << (2 * (i + 1)))) / (one << (2 * (i)));
		n[0] = (n[0] + alpha_prime[0] * (alpha_prime[0] - 1) / 2) % 2;
		alpha_prime[1] = alpha_prime[1] = (alpha[1] % (one << (2 * (i + 1)))) / (one << (2 * (i)));
		n[1] = (n[1] + alpha_prime[1] * (alpha_prime[1] - 1) / 2) % 2;
		alpha_prime[2] = alpha_prime[2] = (alpha[2] % (one << (2 * (i + 1)))) / (one << (2 * (i)));
		n[2] = (n[2] + alpha_prime[2] * (alpha_prime[2] - 1) / 2) % 2;
		alpha_prime[3] = alpha_prime[3] = (alpha[3] % (one << (2 * (i + 1)))) / (one << (2 * (i)));
		n[3] = (n[3] + alpha_prime[3] * (alpha_prime[3] - 1) / 2) % 2;*/
	}
	for (int j = 1; j < 65536; j++) {
		printf("\b\b\b\b\b\b\b\b"); printf("j: %d",j);
		for (int i = 0; i < 65536; i++) {
			if ((i^j)>i) {
				difference[j][0] += antidiagonal1[i] * antidiagonal1[i ^ j];
				difference[j][1] += antidiagonal2[i] * antidiagonal2[i ^ j];
				difference[j][2] += antidiagonal3[i] * antidiagonal3[i ^ j];
				difference[j][3] += antidiagonal4[i] * antidiagonal4[i ^ j];
			}
		}

	}
/*	for (int j = 0; j < 14; j++) {
		printf("Progress: %d/13\n", j);
		if (j < 4) {
			for (bit64 i = 0; i < 65536; i++) {
				alpha_prime[j] = (alpha[j] % (4 * (i + 1))) / (4*i+1);
				n[j] = (n[j] + alpha_prime[j] * (alpha_prime[j] - 1) / 2) % 2;
			}
			printf("n[%d]: %I64d\n", j, n[j]);
		}
		else if (j < 10) {
			for (bit64 i = 0; i < 4294967296; i++) {
				alpha_prime[j] = (alpha[j] % (4 * (i + 1))) & 0x3;
				n[j] = (n[j] + alpha_prime[j] * (alpha_prime[j] - 1) / 2) % 2;
			}
			printf("n[%d]: %I64d\n", j, n[j]);
		}
		else {
			for (bit64 i = 0; i < 281474976710656; i++) {
				alpha_prime[j] = (alpha[j] % (4 * (i + 1))) & 0x3;
				n[j] = (n[j] + alpha_prime[j] * (alpha_prime[j] - 1) / 2) % 2;
			}
			printf("n[%d]: %I64d\n", j, n[j]);
		}
	}*/
/*	for (int j = 0; j < 14; j++) {
		printf("Progress: %d/13\n", j);
		if (j < 4) {
			for (bit64 i = 0; i < 65536; i++) {
				alpha_prime[j] = (alpha[j] % (4 * (i + 1))) / (4 * i);
				n[j] = (n[j] + alpha_prime[j] * (alpha_prime[j] - 1) / 2) % 2;
			}
			printf("n[%d]: %I64d\n",j,n[j]);
		}
		else if (j<10) {
			for (bit64 i = 0; i < 4294967296; i++) {
				alpha_prime[j] = (alpha[j] % (4 * (i + 1))) / (4 * i);
				n[j] = (n[j] + alpha_prime[j] * (alpha_prime[j] - 1) / 2) % 2;
			}
			printf("n[%d]: %I64d\n", j, n[j]);
		}
		else {
			for (bit64 i = 0; i < 281474976710656; i++) {
				alpha_prime[j] = (alpha[j] % (4 * (i + 1))) / (4 * i);
				n[j] = (n[j] + alpha_prime[j] * (alpha_prime[j] - 1) / 2) % 2;
			}
			printf("n[%d]: %I64d\n", j, n[j]);
		}
	}*/
	printf("\b\b\b\b\b\b\b\b");
	printf("Anti-diagonal 1: %I64d\n", hits[0]);
	printf("Anti-diagonal 2: %I64d\n", hits[1]);
	printf("Anti-diagonal 3: %I64d\n", hits[2]);
	printf("Anti-diagonal 4: %I64d\n", hits[3]);

	printf("XOR1 1: %I64d\n", xor1[0]);
	printf("XOR1 2: %I64d\n", xor1[1]);
	printf("XOR1 3: %I64d\n", xor1[2]);
	printf("XOR1 4: %I64d\n", xor1[3]);
	printf("Integral: %016I64x\n", integral);
	for (int j = 1; j < 65536; j++) {
		if ((difference[j][0] % 2) != 0) failcount++;
		if ((difference[j][1] % 2) != 0) failcount++;
		if ((difference[j][2] % 2) != 0) failcount++;
		if ((difference[j][3] % 2) != 0) failcount++;
	}
	printf("Fail count: %I64d\n",failcount);
/*	for (int i = 0; i < 16384; i++) {
		for (int j = 0; j < 65536; j++) {
			if ((antidiagonal12[i][j] % 128)) {
				printf("Exception found for antidiagonal 1+2: %d %d\n",i,j);
				j = 50000000; i = 50000000;
			}
		}
	}*/
/*	for (int i = 0; i < 4294967296; i++) {
		if ((antidiagonal12[i] % 128)) {
			printf("Exception found for antidiagonal 1+2\n");
			break;
		}
	}*/
/*	for (int j = 0; j < 14; j++) {
		printf("Progress: %d/13\n", j);
		if (j < 4) {
			for (bit64 i = 0; i < 65536; i++) {
				if (i) alpha_prime[j] = (alpha[j] % (4 * (i + 1))) / (4 * i);
				else alpha_prime[j] = (alpha[j] % (4 * (i + 1)));
				n[j] = (n[j] + alpha_prime[j] * (alpha_prime[j] - 1) / 2) % 2;
			}
			printf("n[%d]: %I64d\n", j, n[j]);
		}
		else if (j < 10) {
			for (bit64 i = 0; i < 4294967296; i++) {
				if (i) alpha_prime[j] = (alpha[j] % (4 * (i + 1))) / (4 * i);
				else alpha_prime[j] = (alpha[j] % (4 * (i + 1)));
				n[j] = (n[j] + alpha_prime[j] * (alpha_prime[j] - 1) / 2) % 2;
			}
			printf("n[%d]: %I64d\n", j, n[j]);
		}
		else {
			for (bit64 i = 0; i < 281474976710656; i++) {
				if (i) alpha_prime[j] = (alpha[j] % (4 * (i + 1))) / (4 * i);
				else alpha_prime[j] = (alpha[j] % (4 * (i + 1)));
				n[j] = (n[j] + alpha_prime[j] * (alpha_prime[j] - 1) / 2) % 2;
			}
			printf("n[%d]: %I64d\n", j, n[j]);
		}
	}
	printf("n: ");
	for (int j = 0; j < 14; j++) printf("%I64d ", n[j]);
	printf("\n");*/
//	fclose(f1); fclose(f2); fclose(f3); fclose(f4);
//	free(antidiagonal12);
}
void on_the_fly_improved(bit64 key, int round) {  // This time we are checking Lorenzo's multiple-of-2 property for any N bytes
	bit64 plaintext = 0x0000000000000000, ciphertext, temp[14] = { 0 }, input, bytes[16] = { 0 }, index=0, temp2=0;
	key_schedule(key);
	for (__int64 i = 0; i <= 0xffffffff; i++) {
		if ((i % 0x1000000) == 0) { printf("\b\b\b\b\b\b\b\b"); printf("Step: %I64x", i / 0x1000000); }
		ciphertext = encrypt(plaintext, round);
		integral ^= ciphertext;
		input = ciphertext;
		temp[0] = ((input & 0xf000) >> 12) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf000000000000) >> 36);
		temp[1] = ((input & 0xf) >> 0) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf0000000000000) >> 40);
		temp[2] = ((input & 0xf0) >> 4) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf00000000000000) >> 44);
		temp[3] = ((input & 0xf00) >> 8) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf000000000000000) >> 48);
		for (int j = 0; j < 16; j++) {
			bytes[j] = (input >> j * 4) & 0xf;
			byte1[j][bytes[j]]++;
		}
		index = 0;
		for (int j = 0; j < 16; j++) {
			for (int k = j+1; k < 16; k++) {
				temp2 = ((bytes[j] << 4) ^ bytes[k]) & 0xff;
				byte2[index][temp2]++;
				index++;
			}
		}
		index = 0;
		for (int j = 0; j < 16; j++) {
			for (int k = j + 1; k < 16; k++) {
				for (int l = k + 1; l < 16; l++) {
					temp2 = ((bytes[j] << 8) ^ (bytes[k] << 4) ^ bytes[l]) & 0xfff;
					byte3[index][temp2]++;
					index++;
				}
			}
		}
		index = 0;
		for (int j = 0; j < 16; j++) {
			for (int k = j + 1; k < 16; k++) {
				for (int l = k + 1; l < 16; l++) {
					for (int m = l + 1; m < 16; m++) {
						temp2 = ((bytes[j] << 12) ^ (bytes[k] << 8) ^ (bytes[l] << 4) ^ bytes[m]) & 0xffff;
						byte4[index][temp2]++;
						index++;
					}
				}
			}
		}
		antidiagonal1[temp[0]]++;
		antidiagonal2[temp[1]]++;
		antidiagonal3[temp[2]]++;
		antidiagonal4[temp[3]]++;
		plaintext += 0x0000000100000001;
	}
	for (int i = 0; i < 65536; i++) {
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
	}
	printf("\b\b\b\b\b\b\b\b");
	printf("Anti-diagonal 1: %I64d\n", hits[0]);
	printf("Anti-diagonal 2: %I64d\n", hits[1]);
	printf("Anti-diagonal 3: %I64d\n", hits[2]);
	printf("Anti-diagonal 4: %I64d\n", hits[3]);
	printf("Integral: %016I64x\n", integral);
	for (int i = 0; i < 16; i++) { 
		for (int j = 0; j < 16; j++) {
			hits1[i] = hits1[i] + (byte1[i][j] * (byte1[i][j]-1))/2;
		}
		printf("Byte1[%d]: %I64d\n",i,hits1[i]);
	}
	for (int i = 0; i < 120; i++) {
		for (int j = 0; j < 256; j++) {
			hits2[i] = hits2[i] + (byte2[i][j] * (byte2[i][j] - 1)) / 2;
		}
		printf("Byte2[%d]: %I64d\n", i, hits2[i]);
	}
	for (int i = 0; i < 560; i++) {
		for (int j = 0; j < 4096; j++) {
			hits3[i] = hits3[i] + (byte3[i][j] * (byte3[i][j] - 1)) / 2;
		}
		if (hits3[i] % 2 == 1) printf("Byte3[%d]: %I64d\n", i, hits3[i]);
	}
	for (int i = 0; i < 1820; i++) {
		for (int j = 0; j < 65536; j++) {
			hits4[i] = hits4[i] + (byte4[i][j] * (byte4[i][j] - 1)) / 2;
		}
		if (hits4[i] % 2 == 1) printf("Byte4[%d]: %I64d\n", i, hits4[i]);
	}
}
void on_the_fly16(bit64 key, int round) {
	bit64 plaintext = 0x0000000000000000, ciphertext, temp[14] = { 0 }, input;
	key_schedule(key);
	for (__int64 i = 0; i <= 0xffff; i++) {
		if ((i % 0x1000000) == 0) { printf("\b\b\b\b\b\b\b\b"); printf("Step: %I64x", i / 0x1000000); }
		ciphertext = encrypt(plaintext, round);
		integral ^= ciphertext;
		input = ciphertext;
		temp[0] = ((input & 0xf000) >> 12) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf000000000000) >> 36);
		temp[1] = ((input & 0xf) >> 0) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf0000000000000) >> 40);
		temp[2] = ((input & 0xf0) >> 4) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf00000000000000) >> 44);
		temp[3] = ((input & 0xf00) >> 8) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal1[temp[0]]++;
		antidiagonal2[temp[1]]++;
		antidiagonal3[temp[2]]++;
		antidiagonal4[temp[3]]++;
		plaintext += 0x0001000100010001;
	}
	for (int i = 0; i < 65536; i++) {
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
	}
	printf("\b\b\b\b\b\b\b\b");
	printf("Anti-diagonal 1: %I64d\n", hits[0]);
	printf("Anti-diagonal 2: %I64d\n", hits[1]);
	printf("Anti-diagonal 3: %I64d\n", hits[2]);
	printf("Anti-diagonal 4: %I64d\n", hits[3]);
	printf("Integral: %016I64x\n", integral);
}
void on_the_fly_decryption(bit64 key, int round) {
	bit64 plaintext = 0x0000000000000000, ciphertext, temp, input;
	key_schedule_inverse(key,round);
	for (__int64 i = 0; i <= 0xffffffff; i++) {
		if ((i % 0x1000000) == 0) { printf("\b\b\b\b\b\b\b\b"); printf("Step: %I64x", i / 0x1000000); }
		ciphertext = decrypt(plaintext, round);
		integral ^= ciphertext;
		input = ciphertext;
/*		temp = ((input & 0xf000) >> 12) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf000000000000) >> 36);
		antidiagonal1[temp]++;
		temp = ((input & 0xf) >> 0) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf0000000000000) >> 40);
		antidiagonal2[temp]++;
		temp = ((input & 0xf0) >> 4) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf00000000000000) >> 44);
		antidiagonal3[temp]++;
		temp = ((input & 0xf00) >> 8) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal4[temp]++;*/
		// We are now checking diagonal //
		temp = ((input & 0xf000) >> 12) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf00000000000000) >> 44);
		antidiagonal1[temp]++;
		temp = ((input & 0xf) >> 0) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal2[temp]++;
		temp = ((input & 0xf0) >> 4) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf000000000000) >> 36);
		antidiagonal3[temp]++;
		temp = ((input & 0xf00) >> 8) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf0000000000000) >> 40);
		antidiagonal4[temp]++; 
		plaintext += 0x0000000100000001;
	}
	for (int i = 0; i < 65536; i++) {
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
	}
	printf("\b\b\b\b\b\b\b\b");
	printf("Anti-diagonal 1: %I64d\n", hits[0]);
	printf("Anti-diagonal 2: %I64d\n", hits[1]);
	printf("Anti-diagonal 3: %I64d\n", hits[2]);
	printf("Anti-diagonal 4: %I64d\n", hits[3]);
	printf("Integral: %016I64x\n", integral);
}
void on_the_fly128(bit64 key[2], int round) {
	bit64 plaintext = 0x0000000000000000, ciphertext, temp, input;
	key_schedule128(key);
	for (__int64 i = 0; i <= 0xffffffff; i++) {
		if ((i % 0x1000000) == 0) { printf("\b\b\b\b\b\b\b\b"); printf("Step: %I64x", i / 0x1000000); }
		ciphertext = encrypt(plaintext, round);
		integral ^= ciphertext;
		input = ciphertext;
		temp = ((input & 0xf000) >> 12) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf000000000000) >> 36);
		antidiagonal1[temp]++;
		temp = ((input & 0xf) >> 0) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf0000000000000) >> 40);
		antidiagonal2[temp]++;
		temp = ((input & 0xf0) >> 4) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf00000000000000) >> 44);
		antidiagonal3[temp]++;
		temp = ((input & 0xf00) >> 8) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal4[temp]++;
		plaintext += 0x0000000100000001;
	}
	for (int i = 0; i < 65536; i++) {
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
	}
	printf("\b\b\b\b\b\b\b\b");
	printf("Anti-diagonal 1: %I64d\n", hits[0]);
	printf("Anti-diagonal 2: %I64d\n", hits[1]);
	printf("Anti-diagonal 3: %I64d\n", hits[2]);
	printf("Anti-diagonal 4: %I64d\n", hits[3]);
	printf("Integral: %016I64x\n", integral);
}
void on_the_fly128_decryption(bit64 key[2], int round) {
	bit64 plaintext = 0x0000000000000000, ciphertext, temp, input;
	key_schedule128_inverse(key,round);
	for (__int64 i = 0; i <= 0xffffffff; i++) {
		if ((i % 0x1000000) == 0) { printf("\b\b\b\b\b\b\b\b"); printf("Step: %I64x", i / 0x1000000); }
		ciphertext = decrypt(plaintext, round);
		integral ^= ciphertext;
		input = ciphertext;
/*		temp = ((input & 0xf000) >> 12) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf000000000000) >> 36);
		antidiagonal1[temp]++;
		temp = ((input & 0xf) >> 0) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf0000000000000) >> 40);
		antidiagonal2[temp]++;
		temp = ((input & 0xf0) >> 4) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf00000000000000) >> 44);
		antidiagonal3[temp]++;
		temp = ((input & 0xf00) >> 8) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal4[temp]++;*/
		// We are now checking diagonal //
		temp = ((input & 0xf000) >> 12) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf00000000000000) >> 44);
		antidiagonal1[temp]++;
		temp = ((input & 0xf) >> 0) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal2[temp]++;
		temp = ((input & 0xf0) >> 4) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf000000000000) >> 36);
		antidiagonal3[temp]++;
		temp = ((input & 0xf00) >> 8) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf0000000000000) >> 40);
		antidiagonal4[temp]++;
		plaintext += 0x0000000100000001;
	}
	for (int i = 0; i < 65536; i++) {
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
	}
	printf("\b\b\b\b\b\b\b\b");
	printf("Anti-diagonal 1: %I64d\n", hits[0]);
	printf("Anti-diagonal 2: %I64d\n", hits[1]);
	printf("Anti-diagonal 3: %I64d\n", hits[2]);
	printf("Anti-diagonal 4: %I64d\n", hits[3]);
	printf("Integral: %016I64x\n", integral);
}
void generate_ciphertexts_no_whitening(bit64 key, int round) {
	FILE *fp;
	bit64 plaintext = 0x0000000000000000, ciphertext;
	key_schedule(key);
	fopen_s(&fp, "ciphertext.txt", "w");
	//	for (int i = 0; i < 10; i++) printf("%d: %016I64x\n",i, rkey[i]);
	for (__int64 i = 0; i <= 0xffffffff; i++) {
		if ((i % 0x1000000) == 0) printf("Step: %I64x\n", i);
		ciphertext = encrypt_no_whitening(plaintext, round);
		fprintf(fp, "%016I64x\n", ciphertext);
		plaintext += 0x0000000100000001;
		//		printf("Encrypt(%016I64x) = %016I64x\n", plaintext, ciphertext);
	}
	//	printf("%016I64x (Plaintext)\n", plaintext);
	//	ciphertext = encrypt(plaintext, 1);
	//	printf("Encrypt(%016I64x) = %016I64x\n", plaintext, ciphertext);
	fclose(fp);

}
void read_ciphertexts() {
	FILE *fp, *fp2;
	bit64 input, difference, counter=0;
	fopen_s(&fp, "ciphertext.txt", "r");
	for (__int64 i = 0; i < data2; i++) {	
		if ((i % 0x1000000) == 0) printf("Count: %I64d\n", counter);
		fscanf_s(fp, "%016I64x", &input);
		fopen_s(&fp2, "ciphertext.txt", "r");
		for (__int64 j = 0; j < data2; j++) {
			fscanf_s(fp2, "%016I64x", &difference);
			if (j>i) {
				difference ^= input;
				if ((difference & 0x000f00f00f00f000) == 0) counter++;
				else if ((difference & 0x00f00f00f000000f) == 0) counter++;
				else if ((difference & 0x0f00f000000f00f0) == 0) counter++;
				else if ((difference & 0xf000000f00f00f00) == 0) counter++;
			}
		}
		fclose(fp2);
	}
	printf("Counter: %I64d\n", counter);
	fclose(fp);
}
void read_data1() { for (int i = 0; i < datasize; i++) 	fscanf_s(fp, "%016I64x", &input1[i]); block1++;  }
void read_data2() { for (int i = 0; i < datasize; i++) 	fscanf_s(fp2, "%016I64x", &input2[i]); block2++;  }
void read_ciphertexts2() {
	bit64  difference, counter = 0;
	fopen_s(&fp, "ciphertext.txt", "r");
	for (int i = 0; i < data/datasize; i++) {
		printf("%05d: Count = %I64d\n", i,counter);
		read_data1();
		fopen_s(&fp2, "ciphertext.txt", "r");
		for (int j = 0; j < data / datasize; j++) {
			printf("\t%05d: Count = %I64d\n", j, counter);
			read_data2();
			if (block1 == block2) {
				for (int t = 0; t < datasize; t++) {
					if (t % 0x80000==0) printf("\t\t%05d: Count = %I64d\n", t, counter);
					for (int m = 0; m < datasize; m++) {
						if (m>t) {
							difference = input1[t] ^ input2[m];
							if ((difference & 0x000f00f00f00f000) == 0) counter++;
							else if ((difference & 0x00f00f00f000000f) == 0) counter++;
							else if ((difference & 0x0f00f000000f00f0) == 0) counter++;
							else if ((difference & 0xf000000f00f00f00) == 0) counter++;
						}
					}
				}
			}
			else if (block1 < block2) {
				for (int t = 0; t < datasize; t++) {
					if (t % 0x80000==0) printf("\t\t%05d: Count = %I64d\n", t, counter);
					for (int m = 0; m < datasize; m++) {
						difference = input1[t] ^ input2[m];
						if ((difference & 0x000f00f00f00f000) == 0) counter++;
						else if ((difference & 0x00f00f00f000000f) == 0) counter++;
						else if ((difference & 0x0f00f000000f00f0) == 0) counter++;
						else if ((difference & 0xf000000f00f00f00) == 0) counter++;
					}
				}
			}
			//else {read_data2();}
		}
		fclose(fp2);
		block2 = 0;
	}
	printf("Counter: %I64d\n", counter);
	fclose(fp);
	fclose(fp2);
}
void read_ciphertexts3() {
	FILE *fp3, *fp4, *fp5;
	bit64 input, counter = 0, total=0;
	bit64 temp;
//	bit64 temp2;
	fopen_s(&fp, "ciphertext.txt", "r");
	for (__int64 i = 0; i < data; i++) {
		if (i%datasize == 0) printf("Step: %08I64x\n", i);
		fscanf_s(fp, "%016I64x", &input);
		temp = ((input & 0xf000) >> 12) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf000000000000) >> 36);
		antidiagonal1[temp]++;
//		temp2 = temp;
		temp = ((input & 0xf) >> 0) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf0000000000000) >> 40);
		antidiagonal2[temp]++;
//		antidiagonal12[((temp<<8)^temp2)&0xffffffff]++;
		temp = ((input & 0xf0) >> 4) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf00000000000000) >> 44);
		antidiagonal3[temp]++;
//		temp2 = temp;
		temp = ((input & 0xf00) >> 8) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal4[temp]++;
//		antidiagonal34[((temp << 8) ^ temp2) & 0xffffffff]++;

//		if ((difference & 0x000f00f00f00f000) == 0) counter++;
//		else if ((difference & 0x00f00f00f000000f) == 0) counter++;
//		else if ((difference & 0x0f00f000000f00f0) == 0) counter++;
//		else if ((difference & 0xf000000f00f00f00) == 0) counter++;
	}
	fopen_s(&fp2, "results1.txt", "w");
	fopen_s(&fp3, "results2.txt", "w");
	fopen_s(&fp4, "results3.txt", "w");
	fopen_s(&fp5, "results4.txt", "w");
	for (int i = 0; i < 65536; i++) {
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
		fprintf(fp2, "%d\n", antidiagonal1[i]);
		fprintf(fp3, "%d\n", antidiagonal2[i]);
		fprintf(fp4, "%d\n", antidiagonal3[i]);
		fprintf(fp5, "%d\n", antidiagonal4[i]);
	}
	printf("Hits1: %I64d\n", hits[0]);
	printf("Hits2: %I64d\n", hits[1]);
	printf("Hits3: %I64d\n", hits[2]);
	printf("Hits4: %I64d\n", hits[3]);
	fprintf(fp2,"Hits: %I64d\n", hits[0]);
	fprintf(fp3, "Hits: %I64d\n", hits[1]);
	fprintf(fp4, "Hits: %I64d\n", hits[2]);
	fprintf(fp5, "Hits: %I64d\n", hits[3]);
	total = hits[0] + hits[1] + hits[2] + hits[3];
	printf("Total: %I64d\n", total);
	fprintf(fp2,"Total: %I64d\n", total);
	fclose(fp2); fclose(fp3); fclose(fp4); fclose(fp5);
	fclose(fp);
	// double anti-diagonal
/*	hits[0] = 0; hits[1] = 0;
	for (bit32 i = 0; i < 0xffffffff; i++) {
		if (antidiagonal12[i]) hits[0] = hits[0] + antidiagonal12[i] * (antidiagonal12[i] - 1) / 2;	
//		if (antidiagonal34[i]) hits[1] = hits[1] + antidiagonal34[i] * (antidiagonal34[i] - 1) / 2;
	}
	printf("Antidiagonal12: %I64d\n", hits[0]);
//	printf("Antidiagonal34: %I64d\n", hits[1]);*/
}
void print_rkeys() { for (int i = 0; i < 6; i++) printf("Rkey %d: %I64x\n", i, rkey[i]); }
void bug_hunt() {
	bit64 key = 0x357a357a357a357a, temp[4];
	bit64 plaintext = 0x357a357a357a357a, state;
	key_schedule(key);
	print_rkeys();
	for (int i = 0; i < 6; i++) rkey[i] = (rkey[i] & 0xf0000f0000f0000f) ^ ((rkey[i] & 0x0f0000f0000f0000) >> 12) ^ ((rkey[i] & 0x00f0000f00000000) >> 24) ^ ((rkey[i] & 0x000f000000000000) >> 36) ^ ((rkey[i] & 0x0000f0000f0000f0) << 12) ^ ((rkey[i] & 0x00000000f0000f00) << 24) ^ ((rkey[i] & 0x000000000000f000) << 36);
	print_rkeys();
	state = plaintext;
	//	state = (plaintext & 0xf0000f0000f0000f) ^ ((plaintext & 0x0f0000f0000f0000) >> 12) ^ ((plaintext & 0x00f0000f00000000) >> 24) ^ ((plaintext & 0x000f000000000000) >> 36) ^ ((plaintext & 0x0000f0000f0000f0) << 12) ^ ((plaintext & 0x00000000f0000f00) << 24) ^ ((plaintext & 0x000000000000f000) << 36);
	printf("State: %I64x\n", state);
	for (int i = 0; i < 20; i++) {
		plaintext += 0x0000000100000001;
		state = plaintext;
		//	state = (plaintext & 0xf0000f0000f0000f) ^ ((plaintext & 0x0f0000f0000f0000) >> 12) ^ ((plaintext & 0x00f0000f00000000) >> 24) ^ ((plaintext & 0x000f000000000000) >> 36) ^ ((plaintext & 0x0000f0000f0000f0) << 12) ^ ((plaintext & 0x00000000f0000f00) << 24) ^ ((plaintext & 0x000000000000f000) << 36);
		printf("State: %I64x\n", state);
	}
	temp[0] = (state & 0xf000f000f000f000);
	temp[1] = (state & 0x0f000f000f000f00);
	temp[2] = (state & 0x00f000f000f000f0);
	temp[3] = (state & 0x000f000f000f000f);
	temp[1] = ROTL(temp[1], 16);
	temp[2] = ROTL(temp[2], 32);
	temp[3] = ROTL(temp[3], 48);
	state = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
	printf("State: %I64x\n", state);
	temp[0] = (state >> 48) & 0xffff;
	temp[1] = (state >> 32) & 0xffff;
	temp[2] = (state >> 16) & 0xffff;
	temp[3] = (state >> 0) & 0xffff;
	temp[0] = matrix_multiplication(temp[0]);
	temp[1] = matrix_multiplication(temp[1]);
	temp[2] = matrix_multiplication(temp[2]);
	temp[3] = matrix_multiplication(temp[3]);
	state = (temp[0] << 48) ^ (temp[1] << 32) ^ (temp[2] << 16) ^ (temp[3] << 0);
	printf("State: %I64x\n", state);

}
void AESencrypt(bit32 key[], bit32 a[], int round) {
	bit32 temp; int j;
	a[0] ^= key[0];
	a[1] ^= key[1];
	a[2] ^= key[2];
	a[3] ^= key[3];
	for (j = 1; j<round; j++) {
//		printf("Key %d: %08x %08x %08x %08x\n", j, key[0], key[1], key[2], key[3]);
		b[0] = table0[a[0] >> 24] ^ table1[(a[1] >> 16) & 0xff] ^ table2[(a[2] >> 8) & 0xff] ^ table3[a[3] & 0xff];
		b[1] = table0[a[1] >> 24] ^ table1[(a[2] >> 16) & 0xff] ^ table2[(a[3] >> 8) & 0xff] ^ table3[a[0] & 0xff];
		b[2] = table0[a[2] >> 24] ^ table1[(a[3] >> 16) & 0xff] ^ table2[(a[0] >> 8) & 0xff] ^ table3[a[1] & 0xff];
		b[3] = table0[a[3] >> 24] ^ table1[(a[0] >> 16) & 0xff] ^ table2[(a[1] >> 8) & 0xff] ^ table3[a[2] & 0xff];
		temp = SAES[key[3] >> 24] ^ (SAES[key[3] & 0xff] << 8) ^ (SAES[(key[3] >> 8) & 0xff] << 16) ^ (SAES[(key[3] >> 16) & 0xff] << 24);
		temp = temp^key[0] ^ (RconAES[j] << 24);
		key[1] ^= temp;
		key[2] ^= key[1];
		key[3] ^= key[2];
		key[0] = temp;
		a[0] = b[0] ^ key[0];
		a[1] = b[1] ^ key[1];
		a[2] = b[2] ^ key[2];
		a[3] = b[3] ^ key[3];
	}
/*	b[0] = (SAES[a[0] >> 24] << 24) ^ (SAES[(a[1] >> 16) & 0xff] << 16) ^ (SAES[(a[2] >> 8) & 0xff] << 8) ^ SAES[a[3] & 0xff];
	b[1] = (SAES[a[1] >> 24] << 24) ^ (SAES[(a[2] >> 16) & 0xff] << 16) ^ (SAES[(a[3] >> 8) & 0xff] << 8) ^ SAES[a[0] & 0xff];
	b[2] = (SAES[a[2] >> 24] << 24) ^ (SAES[(a[3] >> 16) & 0xff] << 16) ^ (SAES[(a[0] >> 8) & 0xff] << 8) ^ SAES[a[1] & 0xff];
	b[3] = (SAES[a[3] >> 24] << 24) ^ (SAES[(a[0] >> 16) & 0xff] << 16) ^ (SAES[(a[1] >> 8) & 0xff] << 8) ^ SAES[a[2] & 0xff];*/
	b[0] = (SAES[a[0] >> 24] << 24) ^ (SAES[(a[0] >> 16) & 0xff] << 16) ^ (SAES[(a[0] >> 8) & 0xff] << 8) ^ SAES[a[0] & 0xff];
	b[1] = (SAES[a[1] >> 24] << 24) ^ (SAES[(a[1] >> 16) & 0xff] << 16) ^ (SAES[(a[1] >> 8) & 0xff] << 8) ^ SAES[a[1] & 0xff];
	b[2] = (SAES[a[2] >> 24] << 24) ^ (SAES[(a[2] >> 16) & 0xff] << 16) ^ (SAES[(a[2] >> 8) & 0xff] << 8) ^ SAES[a[2] & 0xff];
	b[3] = (SAES[a[3] >> 24] << 24) ^ (SAES[(a[3] >> 16) & 0xff] << 16) ^ (SAES[(a[3] >> 8) & 0xff] << 8) ^ SAES[a[3] & 0xff];
/*	temp = SAES[key[3] >> 24] ^ (SAES[key[3] & 0xff] << 8) ^ (SAES[(key[3] >> 8) & 0xff] << 16) ^ (SAES[(key[3] >> 16) & 0xff] << 24);
	temp = temp^key[0] ^ (RconAES[round] << 24);
	key[1] ^= temp;
	key[2] ^= key[1];
	key[3] ^= key[2];
	key[0] = temp;
	a[0] = b[0] ^ key[0];
	a[1] = b[1] ^ key[1];
	a[2] = b[2] ^ key[2];
	a[3] = b[3] ^ key[3];*/
	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
	a[3] = b[3];
}
void on_the_fly_AES32(bit32 key_real[4], int round) {
	bit32 plaintext[4] = { 0 }, a[4] = { 0 }, zerosum[4] = { 0 }, key[4];
	for (__int64 i = 0; i <= 0xffffffff; i++) {
//	for (__int64 i = 0; i <= 0; i++) {
		if ((i % 0x1000000) == 0) { printf("\b\b\b\b\b\b\b\b"); printf("Step: %I64x", i / 0x1000000); }
		key[0] = key_real[0]; key[1] = key_real[1]; key[2] = key_real[2]; key[3] = key_real[3];
/*		a[0] = (plaintext[0] & 0xff000000) | ((plaintext[1] & 0xff000000)>>8) | ((plaintext[2] & 0xff000000)>>16) | ((plaintext[3] & 0xff000000)>>24);
		a[1] = ((plaintext[0] & 0xff0000)<<8) | ((plaintext[1] & 0xff0000)) | ((plaintext[2] & 0xff0000) >> 8) | ((plaintext[3] & 0xff0000) >> 16);
		a[2] = ((plaintext[0] & 0xff00)<<16) | ((plaintext[1] & 0xff00) <<8 ) | ((plaintext[2] & 0xff00)) | ((plaintext[3] & 0xff00) >> 8);
		a[3] = ((plaintext[0] & 0xff)<<24) | ((plaintext[1] & 0xff) << 16) | ((plaintext[2] & 0xff) << 8) | ((plaintext[3] & 0xff));*/
/*		a[0] = (plaintext[0] & 0xff000000) | (plaintext[1] & 0xff0000) | (plaintext[2] & 0xff00) | (plaintext[3] & 0xff);
		a[1] = ((plaintext[0] & 0xff0000) << 8) | ((plaintext[1] & 0xff00) << 8) | ((plaintext[2] & 0xff) << 8) | ((plaintext[3] & 0xff000000) >> 24);
		a[2] = ((plaintext[0] & 0xff00) << 16) | ((plaintext[1] & 0xff) << 16) | ((plaintext[2] & 0xff000000) >> 16) | ((plaintext[3] & 0xff0000) >> 16);
		a[3] = ((plaintext[0] & 0xff) << 24) | ((plaintext[1] & 0xff000000) >> 8) | ((plaintext[2] & 0xff0000) >> 8) | ((plaintext[3] & 0xff00) >> 8);*/
		a[0] = plaintext[0]; a[1] = plaintext[1]; a[2] = plaintext[2]; a[3] = plaintext[3];
		AESencrypt(key, a, round);
		zerosum[0] ^= a[0]; zerosum[1] ^= a[1]; zerosum[2] ^= a[2]; zerosum[3] ^= a[3];
/*		temp[0] = ((input & 0xf000) >> 12) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf000000000000) >> 36);
		temp[1] = ((input & 0xf) >> 0) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf0000000000000) >> 40);
		temp[2] = ((input & 0xf0) >> 4) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf00000000000000) >> 44);
		temp[3] = ((input & 0xf00) >> 8) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal1[temp[0]]++;
		antidiagonal2[temp[1]]++;
		antidiagonal3[temp[2]]++;
		antidiagonal4[temp[3]]++;*/
		plaintext[0]++; plaintext[1]++; plaintext[2]++; plaintext[3]++;
	}
/*	for (int i = 0; i < 65536; i++) {
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
	}
	printf("\b\b\b\b\b\b\b\b");
	printf("Anti-diagonal 1: %I64d\n", hits[0]);
	printf("Anti-diagonal 2: %I64d\n", hits[1]);
	printf("Anti-diagonal 3: %I64d\n", hits[2]);
	printf("Anti-diagonal 4: %I64d\n", hits[3]);*/
	printf("\b\b\b\b\b\b\b\b");
	printf("Integral: %08x %08x %08x %08x\n", zerosum[0], zerosum[1], zerosum[2], zerosum[3] );
}
void user_input() {
	FILE *fp;
	int choice, keyselection=-1, rounds;
	bit64 key = 0x357a357a357a357a, key128[2] = { 0 };
	bit64 keys[5] = { 0x357a357a357a357a, 0xf903f903f903f903, 0x0000000000000000, 0x5555555555555555 };
	bit32 AESkey[4] = { 0 };
	printf(""
		"(1) Generate 6-round ciphertext\n"
		"(2) Check ciphertext\n"
		"(3) Old check ciphertext\n"
		"(4) Check Ciphertext (FINAL)\n"
		"(5) Debug\n"
		"(6) Generate 5-round ciphertexts\n"
		"(7) Generate 4-round ciphertexts\n"
		"(8) Generate 6-round ciphertexts (no WK)\n"
		"\n"
		"(9) Experiment on the fly\n"
		"(10) Experiment on the fly 128-bit key\n"
		"(11) Key schedule 128 test\n"
		"(12) AES-192 Key experiment\n"
		"(13) Experiment on the fly (decryption)\n"
		"(14) Experiment on the fly 128-bit key (decryption)\n"
		"(15) Experiment on the fly (2^16 plaintexts)\n"
		"\n"
		"(16) Experiment on the fly REAL AES (2^32 plaintexts)\n"
		"\n"
		"(17) Multiple-of-2 check for N bytes\n"
		"Choice: ");
	scanf_s("%d", &choice);
	if (choice == 1) {
		printf("(0) %I64x\n(1) %I64x\n(2) %I64x\n(3) %I64x\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
		scanf_s("%d", &keyselection);
		generate_ciphertexts(keys[keyselection], 6);
	}
	else if (choice == 2) {
		//		printf("Data to read (Enter 0 for full codebook): ");
		//		scanf_s("%I64d",&data);
		read_ciphertexts2();
	}
	else if (choice == 3) {
		printf("Data to read (Enter 0 for full codebook): ");
		scanf_s("%I64d", &data2);
		read_ciphertexts();
	}
	else if (choice == 4) {	read_ciphertexts3();	}
	else if (choice == 5) {	bug_hunt();	}
	else if (choice == 6) {
		printf("(0) %I64x\n(1) %I64x\n(2) %I64x\n(3) %I64x\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
		scanf_s("%d", &keyselection);
		generate_ciphertexts(keys[keyselection], 5);
	}
	else if (choice == 7) {
		printf("(0) %I64x\n(1) %I64x\n(2) %I64x\n(3) %I64x\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
		scanf_s("%d", &keyselection);
		generate_ciphertexts(keys[keyselection], 4);
	}
	else if (choice == 8) {
		printf("(0) %I64x\n(1) %I64x\n(2) %I64x\n(3) %I64x\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
		scanf_s("%d", &keyselection);
		generate_ciphertexts_no_whitening(keys[keyselection], 6);
	}
	else if (choice == 9) {
		printf("Number of rounds: ");
		scanf_s("%d", &rounds);
		printf("(0) %I64x\n(1) %I64x\n(2) %I64x\n(3) %I64x\n(4) Enter manually\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
		scanf_s("%d", &keyselection);
		if (keyselection == 4) {
			printf("Enter key (e.g. 0123456789abcdef): ");
			scanf_s("%016I64x", &keys[keyselection]);
			printf("Selected key is: %016I64x\n", keys[keyselection]);
		}
		on_the_fly(keys[keyselection], rounds);
	}
	else if (choice == 10) {
		printf("Number of rounds: ");
		scanf_s("%d", &rounds);
		printf("Enter key part 1 (e.g. 0123456789abcdef): ");
		scanf_s("%016I64x", &key128[0]);
		printf("Enter key part 2 (e.g. 0123456789abcdef): ");
		scanf_s("%016I64x", &key128[1]);
		printf("Selected key is: %016I64x %016I64x\n", key128[0], key128[1]);
		on_the_fly128(key128, rounds);
	}
	else if (choice == 11) {
		printf("Enter key part 1 (e.g. 0123456789abcdef): ");
		scanf_s("%016I64x", &key128[0]);
		printf("Enter key part 2 (e.g. 0123456789abcdef): ");
		scanf_s("%016I64x", &key128[1]);
		printf("Selected key is: %016I64x %016I64x\n", key128[0], key128[1]);
		key_schedule128_test(key128);
	}
	else if (choice == 12) {AES192_experiment();}
	else if (choice == 13) {
		printf("Number of rounds: ");
		scanf_s("%d", &rounds);
		printf("(0) %I64x\n(1) %I64x\n(2) %I64x\n(3) %I64x\n(4) Enter manually\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
		scanf_s("%d", &keyselection);
		if (keyselection == 4) {
			printf("Enter key (e.g. 0123456789abcdef): ");
			scanf_s("%016I64x", &keys[keyselection]);
			printf("Selected key is: %016I64x\n", keys[keyselection]);
		}
		on_the_fly_decryption(keys[keyselection], rounds);
	}
	else if (choice == 14) {
		printf("Number of rounds: ");
		scanf_s("%d", &rounds);
		printf("Enter key part 1 (e.g. 0123456789abcdef): ");
		scanf_s("%016I64x", &key128[0]);
		printf("Enter key part 2 (e.g. 0123456789abcdef): ");
		scanf_s("%016I64x", &key128[1]);
		printf("Selected key is: %016I64x %016I64x\n", key128[0], key128[1]);
		on_the_fly128_decryption(key128, rounds);
	}
	else if (choice == 15) {
		printf("Number of rounds: ");
		scanf_s("%d", &rounds);
		printf("(0) %I64x\n(1) %I64x\n(2) %I64x\n(3) %I64x\n(4) Enter manually\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
		scanf_s("%d", &keyselection);
		if (keyselection == 4) {
			printf("Enter key (e.g. 0123456789abcdef): ");
			scanf_s("%016I64x", &keys[keyselection]);
			printf("Selected key is: %016I64x\n", keys[keyselection]);
		}
		on_the_fly16(keys[keyselection], rounds);
	}
	else if (choice == 16) {
		printf("Number of rounds: ");
		scanf_s("%d", &rounds);
		printf("(0) %I64x\n(1) %I64x\n(2) %I64x\n(3) %I64x\n(4) Enter manually\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
		scanf_s("%d", &keyselection);
		if (keyselection == 0) { AESkey[0] = 0x357a357a; AESkey[1] = 0x357a357a; AESkey[2] = 0x357a357a; AESkey[3] = 0x357a357a; }
		if (keyselection == 1) { AESkey[0] = 0xf903f903; AESkey[1] = 0xf903f903; AESkey[2] = 0xf903f903; AESkey[3] = 0xf903f903; }
		if (keyselection == 2) { AESkey[0] = 0; AESkey[1] = 0; AESkey[2] = 0; AESkey[3] = 0; }
		if (keyselection == 3) { AESkey[0] = 0x55555555; AESkey[1] = 0x55555555; AESkey[2] = 0x55555555; AESkey[3] = 0x55555555; }
		if (keyselection == 4) {
			printf("Enter key 0 (e.g. 89abcdef): ");
			scanf_s("%016I64x", &AESkey[0]);
			printf("Enter key 1 (e.g. 89abcdef): ");
			scanf_s("%016I64x", &AESkey[1]);
			printf("Enter key 2 (e.g. 89abcdef): ");
			scanf_s("%016I64x", &AESkey[2]);
			printf("Enter key 3 (e.g. 89abcdef): ");
			scanf_s("%016I64x", &AESkey[3]);
		}
		printf("Selected key is: %08x %08x %08x %08x\n", AESkey[0], AESkey[1], AESkey[2], AESkey[3]);
		on_the_fly_AES32(AESkey, rounds);
	}
	else if (choice == 17) {
		printf("Number of rounds: ");
		scanf_s("%d", &rounds);
		printf("(0) %I64x\n(1) %I64x\n(2) %I64x\n(3) %I64x\n(4) Enter manually\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
		scanf_s("%d", &keyselection);
		if (keyselection == 4) {
			printf("Enter key (e.g. 0123456789abcdef): ");
			scanf_s("%016I64x", &keys[keyselection]);
			printf("Selected key is: %016I64x\n", keys[keyselection]);
		}
		on_the_fly_improved(keys[keyselection], rounds);
	}
	printf("Time: %u seconds\n", clock() / CLOCKS_PER_SEC);
	if (choice == 9 || choice == 10 || choice == 13 || choice == 14){
		if (choice == 9 || choice == 13) fopen_s(&fp, "results.txt", "ab");
		else fopen_s(&fp, "results128.txt", "ab");
		if (choice == 9 || choice == 10) fprintf(fp, "-- ENCRYPTION --\n");
		else fprintf(fp, "-- DECRYPTION --\n");
		fprintf(fp, "Rounds: %d (Choice: %d)\n", rounds, choice);
		if (keyselection>=0) fprintf(fp, "Key: %016I64x\n", keys[keyselection]);
		else fprintf(fp, "Key: %016I64x %016I64x\n", key128[0], key128[1]);
		fprintf(fp, "Anti-diagonal 1: %I64d\n", hits[0]);
		fprintf(fp, "Anti-diagonal 2: %I64d\n", hits[1]);
		fprintf(fp, "Anti-diagonal 3: %I64d\n", hits[2]);
		fprintf(fp, "Anti-diagonal 4: %I64d\n", hits[3]);
		fprintf(fp, "Integral: %016I64x\n", integral);
		fclose(fp);
	}
}
void main() {
	user_input();
/*	bit64 key = 0, plaintext=0x123456789abcdef0, temp[10];
	key_schedule(key);
	printf("Initial: %I64x\n", plaintext);
	plaintext = encrypt(plaintext,5);
	printf("Encryption: %I64x\n", plaintext);
	for (int i = 0; i < 5; i++) temp[i] = rkey[i];
	for (int i = 0; i < 5; i++) rkey[4-i] = temp[i];
	plaintext = decrypt(plaintext,5);
	printf("Decryption: %I64x\n", plaintext);

	printf("\n---- Key Schedule ----\n");
	key_schedule(0);
	for (int i = 0; i < 10; i++) {
		printf("Rkey[%d]: %I64x\n",i,rkey[i]);
	}
	//key_schedule_inverse(0xdfb20006b6f2580c,10);
	key_schedule_inverse(0x357a357a357a357a,10);
	printf("\n---- Key Schedule Inverse ----\n");
	for (int i = 0; i < 10; i++) {
		printf("Rkey[%d]: %I64x\n", i, rkey[i]);
	}	*/
//	bit64 key[2] = { 0 }, key2[2] = { 0x4b957582331c2a7c, 0xf3a66dce9f65476b };
//	bit64 key[2] = { 0 }, key2[2] = { 0xfcc98aaffcc98aaf, 0x388f9ff8388f9ff8 };
/*	bit64 key[2] = { 0 }, key2[2] = { 0x65e0db1b992951b4, 0xea9c743ad213ebc2 };
	key_schedule128(key);
	for (int i = 0; i < 10; i++) printf("%d: %016I64x\n",i,rkey[i]);
	printf("\n\n");
	key_schedule128_inverse(key2,9);
	for (int i = 0; i < 10; i++) printf("%d: %016I64x\n", i, rkey[i]);*/
}
