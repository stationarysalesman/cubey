#include "surface.h"
#include <iostream>
#include "vio.h"

std::vector<std::tuple<int, int> > SurfaceRender::EdgeMappings =
    {std::make_tuple(0, 1), std::make_tuple(1, 2), std::make_tuple(2, 3), std::make_tuple(0, 3),
    std::make_tuple(4, 5), std::make_tuple(5, 6), std::make_tuple(6, 7), std::make_tuple(4, 7),
    std::make_tuple(0, 4), std::make_tuple(1, 5), std::make_tuple(2, 6), std::make_tuple(3, 7)};

SurfaceRender::SurfaceRender(int n, int b, float f, float s) : _psys(n, b),  
	edge_table(std::map<int, std::vector<int> >()), boundary(b+1), idx_count(0), iso_value(f), scale(s)
{
	/* edge table from http://paulbourke.net/geometry/polygonise */
	edge_table[0] = {};
	edge_table[1] = {0,8,3};
	edge_table[2] = {0,1,9};
	edge_table[3] = {1,8,3,9,8,1};
	edge_table[4] = {1,2,10};
	edge_table[5] = {0,8,3,1,2,10};
	edge_table[6] = {9,2,10,0,2,9};
	edge_table[7] = {2,8,3,2,10,8,10,9,8};
	edge_table[8] = {3,11,2};
	edge_table[9] = {0,11,2,8,11,0};
	edge_table[10] = {1,9,0,2,3,11};
	edge_table[11] = {1,11,2,1,9,11,9,8,11};
	edge_table[12] = {3,10,1,11,10,3};
	edge_table[13] = {0,10,1,0,8,10,8,11,10};
	edge_table[14] = {3,9,0,3,11,9,11,10,9};
	edge_table[15] = {9,8,10,10,8,11};
	edge_table[16] = {4,7,8};
	edge_table[17] = {4,3,0,7,3,4};
	edge_table[18] = {0,1,9,8,4,7};
	edge_table[19] = {4,1,9,4,7,1,7,3,1};
	edge_table[20] = {1,2,10,8,4,7};
	edge_table[21] = {3,4,7,3,0,4,1,2,10};
	edge_table[22] = {9,2,10,9,0,2,8,4,7};
	edge_table[23] = {2,10,9,2,9,7,2,7,3,7,9,4};
	edge_table[24] = {8,4,7,3,11,2};
	edge_table[25] = {11,4,7,11,2,4,2,0,4};
	edge_table[26] = {9,0,1,8,4,7,2,3,11};
	edge_table[27] = {4,7,11,9,4,11,9,11,2,9,2,1};
	edge_table[28] = {3,10,1,3,11,10,7,8,4};
	edge_table[29] = {1,11,10,1,4,11,1,0,4,7,11,4};
	edge_table[30] = {4,7,8,9,0,11,9,11,10,11,0,3};
	edge_table[31] = {4,7,11,4,11,9,9,11,10};
	edge_table[32] = {9,5,4};
	edge_table[33] = {9,5,4,0,8,3};
	edge_table[34] = {0,5,4,1,5,0};
	edge_table[35] = {8,5,4,8,3,5,3,1,5};
	edge_table[36] = {1,2,10,9,5,4};
	edge_table[37] = {3,0,8,1,2,10,4,9,5};
	edge_table[38] = {5,2,10,5,4,2,4,0,2};
	edge_table[39] = {2,10,5,3,2,5,3,5,4,3,4,8};
	edge_table[40] = {9,5,4,2,3,11};
	edge_table[41] = {0,11,2,0,8,11,4,9,5};
	edge_table[42] = {0,5,4,0,1,5,2,3,11};
	edge_table[43] = {2,1,5,2,5,8,2,8,11,4,8,5}; 
	edge_table[44] = {10,3,11,10,1,3,9,5,4}; 
	edge_table[45] = {4,9,5,0,8,1,8,10,1,8,11,10};
	edge_table[46] = {5,4,0,5,0,11,5,11,10,11,0,3};
	edge_table[47] = {5,4,8,5,8,10,10,8,11};
	edge_table[48] = {9,7,8,5,7,9};
	edge_table[49] = {9,3,0,9,5,3,5,7,3};
	edge_table[50] = {0,7,8,0,1,7,1,5,7};
	edge_table[51] = {1,5,3,3,5,7};
	edge_table[52] = {9,7,8,9,5,7,10,1,2};
	edge_table[53] = {10,1,2,9,5,0,5,3,0,5,7,3};
	edge_table[54] = {8,0,2,8,2,5,8,5,7,10,5,2};
	edge_table[55] = {2,10,5,2,5,3,3,5,7};
	edge_table[56] = {7,9,5,7,8,9,3,11,2};
	edge_table[57] = {9,5,7,9,7,2,9,2,0,2,7,11};
	edge_table[58] = {2,3,11,0,1,8,1,7,8,1,5,7};
	edge_table[59] = {11,2,1,11,1,7,7,1,5};
	edge_table[60] = {9,5,8,8,5,7,10,1,3,10,3,11};
	edge_table[61] = {5,7,0,5,0,9,7,11,0,1,0,10,11,10,0};
	edge_table[62] = {11,10,0,11,0,3,10,5,0,8,0,7,5,7,0};
	edge_table[63] = {11,10,5,7,11,5};
	edge_table[64] = {10,6,5};
	edge_table[65] = {0,8,3,5,10,6};
	edge_table[66] = {9,0,1,5,10,6};
	edge_table[67] = {1,8,3,1,9,8,5,10,6};
	edge_table[68] = {1,6,5,2,6,1};
	edge_table[69] = {1,6,5,1,2,6,3,0,8};
	edge_table[70] = {9,6,5,9,0,6,0,2,6};
	edge_table[71] = {5,9,8,5,8,2,5,2,6,3,2,8};
	edge_table[72] = {2,3,11,10,6,5};
	edge_table[73] = {11,0,8,11,2,0,10,6,5};
	edge_table[74] = {0,1,9,2,3,11,5,10,6};
	edge_table[75] = {5,10,6,1,9,2,9,11,2,9,8,11};
	edge_table[76] = {6,3,11,6,5,3,5,1,3};
	edge_table[77] = {0,8,11,0,11,5,0,5,1,5,11,6};
	edge_table[78] = {3,11,6,0,3,6,0,6,5,0,5,9};
	edge_table[79] = {6,5,9,6,9,11,11,9,8};
	edge_table[80] = {5,10,6,4,7,8};
	edge_table[81] = {4,3,0,4,7,3,6,5,10};
	edge_table[82] = {1,9,0,5,10,6,8,4,7};
	edge_table[83] = {10,6,5,1,9,7,1,7,3,7,9,4};
	edge_table[84] = {6,1,2,6,5,1,4,7,8};
	edge_table[85] = {1,2,5,5,2,6,3,0,4,3,4,7};
	edge_table[86] = {8,4,7,9,0,5,0,6,5,0,2,6};
	edge_table[87] = {7,3,9,7,9,4,3,2,9,5,9,6,2,6,9};
	edge_table[88] = {3,11,2,7,8,4,10,6,5};
	edge_table[89] = {5,10,6,4,7,2,4,2,0,2,7,11};
	edge_table[90] = {0,1,9,4,7,8,2,3,11,5,10,6};
	edge_table[91] = {9,2,1,9,11,2,9,4,11,7,11,4,5,10,6};
	edge_table[92] = {8,4,7,3,11,5,3,5,1,5,11,6};
	edge_table[93] = {5,1,11,5,11,6,1,0,11,7,11,4,0,4,11};
	edge_table[94] = {0,5,9,0,6,5,0,3,6,11,6,3,8,4,7};
	edge_table[95] = {6,5,9,6,9,11,4,7,9,7,11,9};
	edge_table[96] = {10,4,9,6,4,10};
	edge_table[97] = {4,10,6,4,9,10,0,8,3};
	edge_table[98] = {10,0,1,10,6,0,6,4,0};
	edge_table[99] = {8,3,1,8,1,6,8,6,4,6,1,10};
	edge_table[100] = {1,4,9,1,2,4,2,6,4};
	edge_table[101] = {3,0,8,1,2,9,2,4,9,2,6,4};
	edge_table[102] = {0,2,4,4,2,6};
	edge_table[103] = {8,3,2,8,2,4,4,2,6};
	edge_table[104] = {10,4,9,10,6,4,11,2,3};
	edge_table[105] = {0,8,2,2,8,11,4,9,10,4,10,6};
	edge_table[106] = {3,11,2,0,1,6,0,6,4,6,1,10};
	edge_table[107] = {6,4,1,6,1,10,4,8,1,2,1,11,8,11,1};
	edge_table[108] = {9,6,4,9,3,6,9,1,3,11,6,3};
	edge_table[109] = {8,11,1,8,1,0,11,6,1,9,1,4,6,4,1};
	edge_table[110] = {3,11,6,3,6,0,0,6,4};
	edge_table[111] = {6,4,8,11,6,8};
	edge_table[112] = {7,10,6,7,8,10,8,9,10};
	edge_table[113] = {0,7,3,0,10,7,0,9,10,6,7,10};
	edge_table[114] = {10,6,7,1,10,7,1,7,8,1,8,0};
	edge_table[115] = {10,6,7,10,7,1,1,7,3};
	edge_table[116] = {1,2,6,1,6,8,1,8,9,8,6,7};
	edge_table[117] = {2,6,9,2,9,1,6,7,9,0,9,3,7,3,9};
	edge_table[118] = {7,8,0,7,0,6,6,0,2};
	edge_table[119] = {7,3,2,6,7,2};
	edge_table[120] = {2,3,11,10,6,8,10,8,9,8,6,7};
	edge_table[121] = {2,0,7,2,7,11,0,9,7,6,7,10,9,10,7};
	edge_table[122] = {1,8,0,1,7,8,1,10,7,6,7,10,2,3,11};
	edge_table[123] = {11,2,1,11,1,7,10,6,1,6,7,1};
	edge_table[124] = {8,9,6,8,6,7,9,1,6,11,6,3,1,3,6};
	edge_table[125] = {0,9,1,11,6,7};
	edge_table[126] = {7,8,0,7,0,6,3,11,0,11,6,0};
	edge_table[127] = {7,11,6};
	edge_table[128] = {7,6,11};
	edge_table[129] = {3,0,8,11,7,6};
	edge_table[130] = {0,1,9,11,7,6};
	edge_table[131] = {8,1,9,8,3,1,11,7,6};
	edge_table[132] = {10,1,2,6,11,7};
	edge_table[133] = {1,2,10,3,0,8,6,11,7};
	edge_table[134] = {2,9,0,2,10,9,6,11,7};
	edge_table[135] = {6,11,7,2,10,3,10,8,3,10,9,8};
	edge_table[136] = {7,2,3,6,2,7};
	edge_table[137] = {7,0,8,7,6,0,6,2,0};
	edge_table[138] = {2,7,6,2,3,7,0,1,9};
	edge_table[139] = {1,6,2,1,8,6,1,9,8,8,7,6};
	edge_table[140] = {10,7,6,10,1,7,1,3,7};
	edge_table[141] = {10,7,6,1,7,10,1,8,7,1,0,8};
	edge_table[142] = {0,3,7,0,7,10,0,10,9,6,10,7};
	edge_table[143] = {7,6,10,7,10,8,8,10,9};
	edge_table[144] = {6,8,4,11,8,6};
	edge_table[145] = {3,6,11,3,0,6,0,4,6};
	edge_table[146] = {8,6,11,8,4,6,9,0,1};
	edge_table[147] = {9,4,6,9,6,3,9,3,1,11,3,6};
	edge_table[148] = {6,8,4,6,11,8,2,10,1};
	edge_table[149] = {1,2,10,3,0,11,0,6,11,0,4,6};
	edge_table[150] = {4,11,8,4,6,11,0,2,9,2,10,9};
	edge_table[151] = {10,9,3,10,3,2,9,4,3,11,3,6,4,6,3};
	edge_table[152] = {8,2,3,8,4,2,4,6,2};
	edge_table[153] = {0,4,2,4,6,2};
	edge_table[154] = {1,9,0,2,3,4,2,4,6,4,3,8};
	edge_table[155] = {1,9,4,1,4,2,2,4,6};
	edge_table[156] = {8,1,3,8,6,1,8,4,6,6,10,1};
	edge_table[157] = {10,1,0,10,0,6,6,0,4};
	edge_table[158] = {4,6,3,4,3,8,6,10,3,0,3,9,10,9,3};
	edge_table[159] = {10,9,4,6,10,4};
	edge_table[160] = {4,9,5,7,6,11};
	edge_table[161] = {0,8,3,4,9,5,11,7,6};
	edge_table[162] = {5,0,1,5,4,0,7,6,11};
	edge_table[163] = {11,7,6,8,3,4,3,5,4,3,1,5};
	edge_table[164] = {9,5,4,10,1,2,7,6,11};
	edge_table[165] = {6,11,7,1,2,10,0,8,3,4,9,5};
	edge_table[166] = {7,6,11,5,4,10,4,2,10,4,0,2};
	edge_table[167] = {3,4,8,3,5,4,3,2,5,10,5,2,11,7,6};
	edge_table[168] = {7,2,3,7,6,2,5,4,9};
	edge_table[169] = {9,5,4,0,8,6,0,6,2,6,8,7};
	edge_table[170] = {3,6,2,3,7,6,1,5,0,5,4,0};
	edge_table[171] = {6,2,8,6,8,7,2,1,8,4,8,5,1,5,8};
	edge_table[172] = {9,5,4,10,1,6,1,7,6,1,3,7};
	edge_table[173] = {1,6,10,1,7,6,1,0,7,8,7,0,9,5,4};
	edge_table[174] = {4,0,10,4,10,5,0,3,10,6,10,7,3,7,10};
	edge_table[175] = {7,6,10,7,10,8,5,4,10,4,8,10};
	edge_table[176] = {6,9,5,6,11,9,11,8,9};
	edge_table[177] = {3,6,11,0,6,3,0,5,6,0,9,5};
	edge_table[178] = {0,11,8,0,5,11,0,1,5,5,6,11};
	edge_table[179] = {6,11,3,6,3,5,5,3,1};
	edge_table[180] = {1,2,10,9,5,11,9,11,8,11,5,6};
	edge_table[181] = {0,11,3,0,6,11,0,9,6,5,6,9,1,2,10};
	edge_table[182] = {11,8,5,11,5,6,8,0,5,10,5,2,0,2,5};
	edge_table[183] = {6,11,3,6,3,5,2,10,3,10,5,3};
	edge_table[184] = {5,8,9,5,2,8,5,6,2,3,8,2};
	edge_table[185] = {9,5,6,9,6,0,0,6,2};
	edge_table[186] = {1,5,8,1,8,0,5,6,8,3,8,2,6,2,8};
	edge_table[187] = {1,5,6,2,1,6};
	edge_table[188] = {1,3,6,1,6,10,3,8,6,5,6,9,8,9,6};
	edge_table[189] = {10,1,0,10,0,6,9,5,0,5,6,0};
	edge_table[190] = {0,3,8,5,6,10};
	edge_table[191] = {10,5,6};
	edge_table[192] = {11,5,10,7,5,11};
	edge_table[193] = {11,5,10,11,7,5,8,3,0};
	edge_table[194] = {5,11,7,5,10,11,1,9,0};
	edge_table[195] = {10,7,5,10,11,7,9,8,1,8,3,1};
	edge_table[196] = {11,1,2,11,7,1,7,5,1};
	edge_table[197] = {0,8,3,1,2,7,1,7,5,7,2,11};
	edge_table[198] = {9,7,5,9,2,7,9,0,2,2,11,7};
	edge_table[199] = {7,5,2,7,2,11,5,9,2,3,2,8,9,8,2};
	edge_table[200] = {2,5,10,2,3,5,3,7,5};
	edge_table[201] = {8,2,0,8,5,2,8,7,5,10,2,5};
	edge_table[202] = {9,0,1,5,10,3,5,3,7,3,10,2};
	edge_table[203] = {9,8,2,9,2,1,8,7,2,10,2,5,7,5,2};
	edge_table[204] = {1,3,5,3,7,5};
	edge_table[205] = {0,8,7,0,7,1,1,7,5};
	edge_table[206] = {9,0,3,9,3,5,5,3,7};
	edge_table[207] = {9,8,7,5,9,7};
	edge_table[208] = {5,8,4,5,10,8,10,11,8};
	edge_table[209] = {5,0,4,5,11,0,5,10,11,11,3,0};
	edge_table[210] = {0,1,9,8,4,10,8,10,11,10,4,5};
	edge_table[211] = {10,11,4,10,4,5,11,3,4,9,4,1,3,1,4};
	edge_table[212] = {2,5,1,2,8,5,2,11,8,4,5,8};
	edge_table[213] = {0,4,11,0,11,3,4,5,11,2,11,1,5,1,11};
	edge_table[214] = {0,2,5,0,5,9,2,11,5,4,5,8,11,8,5};
	edge_table[215] = {9,4,5,2,11,3};
	edge_table[216] = {2,5,10,3,5,2,3,4,5,3,8,4};
	edge_table[217] = {5,10,2,5,2,4,4,2,0};
	edge_table[218] = {3,10,2,3,5,10,3,8,5,4,5,8,0,1,9};
	edge_table[219] = {5,10,2,5,2,4,1,9,2,9,4,2};
	edge_table[220] = {8,4,5,8,5,3,3,5,1};
	edge_table[221] = {0,4,5,1,0,5};
	edge_table[222] = {8,4,5,8,5,3,9,0,5,0,3,5};
	edge_table[223] = {9,4,5};
	edge_table[224] = {4,11,7,4,9,11,9,10,11};
	edge_table[225] = {0,8,3,4,9,7,9,11,7,9,10,11};
	edge_table[226] = {1,10,11,1,11,4,1,4,0,7,4,11};
	edge_table[227] = {3,1,4,3,4,8,1,10,4,7,4,11,10,11,4};
	edge_table[228] = {4,11,7,9,11,4,9,2,11,9,1,2};
	edge_table[229] = {9,7,4,9,11,7,9,1,11,2,11,1,0,8,3};
	edge_table[230] = {11,7,4,11,4,2,2,4,0};
	edge_table[231] = {11,7,4,11,4,2,8,3,4,3,2,4};
	edge_table[232] = {2,9,10,2,7,9,2,3,7,7,4,9};
	edge_table[233] = {9,10,7,9,7,4,10,2,7,8,7,0,2,0,7};
	edge_table[234] = {3,7,10,3,10,2,7,4,10,1,10,0,4,0,10};
	edge_table[235] = {1,10,2,8,7,4};
	edge_table[236] = {4,9,1,4,1,7,7,1,3};
	edge_table[237] = {4,9,1,4,1,7,0,8,1,8,7,1};
	edge_table[238] = {4,0,3,7,4,3};
	edge_table[239] = {4,8,7};
	edge_table[240] = {9,10,8,10,11,8};
	edge_table[241] = {3,0,9,3,9,11,11,9,10};
	edge_table[242] = {0,1,10,0,10,8,8,10,11};
	edge_table[243] = {3,1,10,11,3,10};
	edge_table[244] = {1,2,11,1,11,9,9,11,8};
	edge_table[245] = {3,0,9,3,9,11,1,2,9,2,11,9};
	edge_table[246] = {0,2,11,8,0,11};
	edge_table[247] = {3,2,11};
	edge_table[248] = {2,3,8,2,8,10,10,8,9};
	edge_table[249] = {9,10,2,0,9,2};
	edge_table[250] = {2,3,8,2,8,10,0,1,8,1,10,8};
	edge_table[251] = {1,10,2};
	edge_table[252] = {1,3,8,9,1,8};
	edge_table[253] = {0,9,1};
	edge_table[254] = {0,3,8};
	edge_table[255] = {};	
}

void SurfaceRender::init()
{
	p_vertices = std::vector<glm::vec4>(1000);
	p_vertices.push_back(glm::vec4(0.0, 0.0, 0.0, 1.0));
	p_vertices.push_back(glm::vec4(0.0, 0.0, 0.0, 1.0));
	p_vertices.push_back(glm::vec4(0.0, 0.0, 0.0, 1.0));
	p_normals = std::vector<glm::vec4>(1000);
	p_normals.push_back(glm::vec4(0.0, 0.0, 1.0, 0.0));
	p_normals.push_back(glm::vec4(0.0, 0.0, 1.0, 0.0));
	p_normals.push_back(glm::vec4(0.0, 0.0, 1.0, 0.0));
	t_faces = std::vector<glm::uvec3>(1000);
	t_faces.push_back(glm::uvec3(0, 1, 2));
	p_faces = std::vector<unsigned int>(1000);
	p_faces.push_back(0);
}
/* interpolate vertices along an edge of the cube:
	formula provided by http://paulbourke.net/geometry/polygonise */
glm::vec3 SurfaceRender::InterpolateEdge(glm::vec3& v1, glm::vec3& v2)
{
	float p(_psys.ColorField(v1));
	float q(_psys.ColorField(v2));
	float c1 = iso_value - p;
	float c2 = q - p;
//	if (abs(c2) <= 0.00000005)	return v1; /* close enough probs */
	glm::vec3 magic(v2-v1);
	return v1 + ((c1 / c2) * magic);
}
void SurfaceRender::Test() 
{
	ClearGeometry();
	p_vertices.push_back(glm::vec4(0.0, 1.0, 0.0, 1.0));	
	p_normals.push_back(glm::vec4(0.0, 1.0, 0.0, 0.0));	
	p_faces.push_back(0);	
}

void SurfaceRender::ClearGeometry()
{
	p_vertices.clear();
	p_normals.clear();
	p_faces.clear();
	t_faces.clear();
	idx_count = 0; /* reset the index for geometry faces */
}
/* Render a frame of the scene. v slow */
void SurfaceRender::March()
{
	_psys.step(); /* step the particle system */

	ClearGeometry();
	float minx = 0.0;
	float maxx = 0.0;
	float miny = 0.0;
	float maxy = 0.0;
	float minz = 0.0;
	float maxz = 0.0;
	float e = 0.5;	
	_psys.CalculateBounds(minx, miny, minz, maxx, maxy, maxz);
	minx -= e;
	miny -= e;
	minz -= e;
	maxx += e;
	maxy += e;
	maxz += e;
	float x = minx;	
	float t = std::abs(maxx - minx);
	while (x < maxx)
	{

		float j = std::abs(x - minx); 
		float k = j/t;
		k *= 100;
		int n = (int)k;
		std::cout << "\rRendering: " << n << "\% done" << std::flush; 

		float y = miny;
		while (y < maxy)
		{
			float z = maxz;
			while (z > minz) /* flip */
			{
				EvaluateCube(x, y, z);	
				z -= scale;	
			}
			y += scale;
		}
		x += scale;
	}
	std::cout << std::endl << "Done rendering. " << std::endl;

/*	
	std::vector<glm::vec4>::iterator b = vertices.begin();	
	std::vector<glm::vec4>::iterator e = vertices.end();	
	while (b != e)
	{
		std::cout << "vertex: " << *b << std::endl;	

		++b;
	}
*/
}


void SurfaceRender::EvaluateCube(float x, float y, float z)
{
	/* Build the cube geometry */
	std::vector<glm::vec3> cube_vertices(8);	
	cube_vertices[3] = glm::vec3(x, y, z);
	cube_vertices[2] = glm::vec3(x+scale, y, z);
	cube_vertices[6] = glm::vec3(x+scale, y+scale, z);
	cube_vertices[7] = glm::vec3(x, y+scale, z);
	cube_vertices[0] = glm::vec3(x, y, z-scale);
	cube_vertices[1] = glm::vec3(x+scale, y, z-scale);
	cube_vertices[5] = glm::vec3(x+scale, y+scale, z-scale);
	cube_vertices[4] = glm::vec3(x, y+scale, z-scale);

#ifdef DEBUG
	std::cout << "Cube vertices for " << x << ", " << y << ", " << z << std::endl;
	for(int i = 0; i<cube_vertices.size(); ++i)
		std::cout << cube_vertices[i] << std::endl;
#endif

	/* Check the value of the scalar field at each vertex */
	int lookup = 0;
	lookup |= (_psys.ColorField(cube_vertices[0]) >= iso_value) ? 0x1 : 0x0; 
	lookup |= (_psys.ColorField(cube_vertices[1]) >= iso_value) ? 0x2 : 0x0; 
	lookup |= (_psys.ColorField(cube_vertices[2]) >= iso_value) ? 0x4 : 0x0; 
	lookup |= (_psys.ColorField(cube_vertices[3]) >= iso_value) ? 0x8 : 0x0; 
	lookup |= (_psys.ColorField(cube_vertices[4]) >= iso_value) ? 0x10 : 0x0; 
	lookup |= (_psys.ColorField(cube_vertices[5]) >= iso_value) ? 0x20 : 0x0; 
	lookup |= (_psys.ColorField(cube_vertices[6]) >= iso_value) ? 0x40 : 0x0; 
	lookup |= (_psys.ColorField(cube_vertices[7]) >= iso_value) ? 0x80 : 0x0; 

#ifdef DEBUG
	std::cout << "lookup bit vector:" << lookup << std::endl;
#endif

	/* Lookup the list of edges and interpolate each vertex */
	std::vector<int> edges = edge_table[lookup];

#ifdef DEBUG
	std::cout << "edges list..." << std::endl;
	for (int i=0; i<edges.size(); ++i)
		std::cout << edges[i] << std::endl;
#endif

	std::vector<int>::iterator b = edges.begin();
	std::vector<int>::iterator e = edges.end();
	while (b != e)
	{
		glm::vec3 v_a, v_b;
		std::tuple<int, int> t(SurfaceRender::EdgeMappings[*b]);
		v_a = cube_vertices[std::get<0>(t)];	
		v_b = cube_vertices[std::get<1>(t)];
		glm::vec3 interp_vert = InterpolateEdge(v_a, v_b);

#ifdef DEBUG
	std::cout << "interpolated vertex: " << interp_vert << std::endl;
#endif

		/* Interpolate vertex normal */
		glm::vec3 v_norm = glm::normalize(_psys.ColorFieldGradient(interp_vert));

#ifdef DEBUG
	std::cout << "vertex normal: " << v_norm << std::endl;
#endif
		p_vertices.push_back(glm::vec4(interp_vert, 1.0));
		p_normals.push_back(glm::vec4(v_norm, 0.0));
		p_faces.push_back(idx_count);	

		if ((idx_count+1) % 3 == 0)
			t_faces.push_back(glm::uvec3(idx_count-2, idx_count-1, idx_count)); 	
		
		++b;
		++idx_count;
		
	}

}

