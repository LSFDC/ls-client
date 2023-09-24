// blowfish.h2  header file containing random number tables

static DWORD bf_P[NPASS + 2] = {
// justin made
0x56A9B6F3, 0x86823396, 0x878BF800, 0x1E878B08, 
0x66CC9847, 0xDBA430E7, 0x10FFA52B, 0xFA8F50B7, 
0x929B2B53, 0x67ADF71D, 0xF9542854, 0x4FA2349D, 
0x53BFC7B2, 0x900402CE, 0xBB411702, 0x87588F23, 
0x2F8F15E9, 0x0032D852,
		
// original
/*
  0x243f6a88, 0x85a308d3, 0x13198a2e, 0x03707344,
  0xa4093822, 0x299f31d0, 0x082efa98, 0xec4e6c89,
  0x452821e6, 0x38d01377, 0xbe5466cf, 0x34e90c6c,
  0xc0ac29b7, 0xc97c50dd, 0x3f84d5b5, 0xb5470917,
  0x9216d5d9, 0x8979fb1b,
 */
};
static DWORD bf_S[4][256] = {
// justin made
0x819F1D00, 0x280B57B6, 0x4D52B1BE, 0x2128A6D5, 
0x5934B7FE, 0xA1A0D77C, 0x07DD30AA, 0xEB6D4C63, 
0xB626BD9A, 0x40465684, 0xA7FA6F99, 0x1FF54596, 
0x3D0F976E, 0xBB535B64, 0xA830B7F3, 0x5449F8C6, 
0x4846FEE0, 0x4C707E95, 0x7F18AF3F, 0xF3C10D9B, 
0x7E6229A9, 0xAA15079F, 0x961BCFF7, 0x9315DD7C, 
0x38FCD101, 0x3EBABFEA, 0x94BE90A1, 0xBCADEFD2, 
0x1C5C6D90, 0x7FD6DD1E, 0xF18C89B6, 0xC7F0CB55, 
0x841955DD, 0x05F31982, 0x26EC22CE, 0x5A87FA6D, 
0x18BD3151, 0x3897DCCF, 0xBA65E341, 0xE0EBE282, 
0x4ECE8A74, 0xA13B9D6D, 0x169055B6, 0xEE712E3B, 
0x80A5B7DE, 0xD757E4C4, 0xD0D5FF96, 0xDEC423F2, 
0x66FA50E7, 0x43824A8C, 0x72CC3A69, 0x575B7B4D, 
0x5615DE17, 0x6B26261C, 0x63DD9CB7, 0xB1BD9DA4, 
0xEAADC705, 0xB9C910EE, 0x3C7FA0E8, 0xA543F1A1, 
0x9A1C962A, 0xC5F48198, 0x544CEB84, 0x7AA510AB, 
0xDCE8D0FE, 0x151F0093, 0x53BAC722, 0xD83B9139, 
0x2A8A0FE9, 0x13C20566, 0xA242DB2B, 0x289DCCE4, 
0x1B9A9A92, 0x5774188B, 0xD87B8F27, 0xE1325A14, 
0x288F1A73, 0x479FB278, 0x5DCF6C8E, 0xAB839261, 
0xC8D3F612, 0x7DC95AB7, 0xB9B4F9E8, 0xEE283C42, 
0x830CC6D8, 0x507B89CD, 0x75C3AEAC, 0x13899121, 
0xD1A3F24D, 0x792CB635, 0x08740363, 0xC22E38A4, 
0x4B1013E8, 0x5F567966, 0xFB4F8195, 0x618EA934, 
0x58EB9043, 0x6A8F3AE7, 0xB4BCB0BA, 0x8A226D59, 
0x709B01C4, 0x32318242, 0xDD52064A, 0xA483EA8A, 
0x2CB9EFF4, 0x30E2E8ED, 0xCE8A0DCC, 0x3726BB41, 
0xF3ADA16B, 0xFB2AC460, 0x1DEC2CBA, 0xBC964514, 
0x6D0FDF81, 0xEC53AF25, 0x44D0FC9A, 0xBA3A227C, 
0xE337E2CD, 0x990320C3, 0xCBFEF3E5, 0xBA99D9E1, 
0x0CDD61C9, 0x7CC49FB1, 0x18BD9B22, 0x224CC2FB, 
0x4058C4FB, 0x1DFCA478, 0xC5866CEB, 0x9CAB7601, 
0x183194DC, 0xF234A870, 0x5901DC86, 0x805E8CAD, 
0xFBE057D4, 0x85E34351, 0x3DA675AD, 0x64CD4C65, 
0x710D789A, 0x5DA3EB82, 0xF8EDBFA6, 0xB26F6FA2, 
0x12008C88, 0xD3EA0A8C, 0x124E2029, 0x01DD3C0B, 
0x4760FD34, 0x9E1147E8, 0x03404280, 0xDA8F6BEA, 
0x97A65207, 0x16D0FA0C, 0x446C8D61, 0x94FD55E5, 
0x7B4A1389, 0xC49FBC70, 0x7C2A6736, 0xD7AF8176, 
0x7AD4C931, 0x2FD504BE, 0xE4128A75, 0x0B1C8713, 
0x1CBCEBA9, 0xCF1C6B5C, 0x339B3D97, 0xB9CDD044, 
0xC979E237, 0x2CDA47B4, 0xE13F2933, 0x683BE383, 
0x0AA54474, 0xBF98226C, 0x7684B5C3, 0x81EB4857, 
0x66A69BF8, 0x0FEE94FD, 0x5BE369CD, 0x9B586737, 
0x65155F2B, 0x842313DE, 0x07F4CDCA, 0x2E19D9AC, 
0x7059F674, 0xC7E11979, 0x121E5A22, 0xB285152E, 
0x2E0C0A8D, 0x4FBE2D74, 0x04EB877D, 0xB0359334, 
0xF79402CC, 0x75F3B848, 0x46D1DD32, 0xBFB1DC68, 
0x547A67BA, 0xDF48516D, 0x6069E2FB, 0x37717710, 
0xCB46A0DF, 0x0705705B, 0xD80B001E, 0x91EDCDE5, 
0xD7705592, 0x54D1AE99, 0x285FEF34, 0x84AC743F, 
0x0D80FEAD, 0x7F2661A1, 0xC7CCE5B5, 0x6827D6A6, 
0xC7FDF456, 0xBF7A2409, 0x4EDC8C38, 0xC5D69A92, 
0xAC41EE36, 0xCC367C3A, 0x23155C17, 0x145119AA, 
0x15F235C5, 0x0F01C3BC, 0xD1E0DBF8, 0xEC10DA47, 
0xA8987F8A, 0xFF55A016, 0xEDD57344, 0xA69A65F1, 
0xDF8D270F, 0x34A88BB2, 0xC16BCB83, 0xE9584320, 
0x1258B2AA, 0x1674FD26, 0x042B3C3D, 0x1DE3EB7C, 
0xE890AAF4, 0x3E5F7DFB, 0x1E7E5DD9, 0xCAA0D55C, 
0xE9AE9685, 0x13A17389, 0x88FAA6E0, 0x792A8959, 
0x6D2ADEC5, 0x3DF47877, 0xD9089FEB, 0x91E880DB, 
0x1C7C1A2B, 0x05CE033F, 0x794FA150, 0xBA71516A, 
0x4F3BB341, 0x02A9AC37, 0xE11963C7, 0x4D3E658E, 
0xAED1319D, 0xCCFBDC18, 0xC81C5D9A, 0xD1C732BF, 
0x9FC42A98, 0xCC5DF94A, 0x66B1E85F, 0xDE846284, 
0x9CADF8C9, 0x7936BE45, 0x7370AB9F, 0xDD1D4D56, 
0x3CE342AA, 0x6B007090, 0x58D11EB2, 0x55E989CD, 
0xF80061B1, 0x0B61B9B5, 0x8C5BAA50, 0xCE617031, 
0x478BFC78, 0xE0C2102A, 0x9877D6D1, 0xB13DCA49, 
0xB1EB7B65, 0x72ABEE68, 0x03BE3ADC, 0x95C5DD6F, 
0xBEA96600, 0x4A74C9F7, 0x45964FCA, 0xF2912319, 
0xD75D36D3, 0xDED43B6E, 0xD6977C23, 0x402843C0, 
0x935E6265, 0x9935BC17, 0x5F4B596F, 0x18F3B61C, 
0x6B36821D, 0x102DB2A8, 0x17085F26, 0xD18BE374, 
0xC58B1F84, 0xCD05B77A, 0xC67704D0, 0x14556262, 
0x5BA68012, 0x37654325, 0xC5F0D3F4, 0x57FCAB6C, 
0x653F6D6F, 0xD6C4EC10, 0x9B2B511B, 0x05C737FB, 
0xA99E2FD2, 0x33BC0CE5, 0xC060089D, 0xB35D9DA7, 
0x716A5D04, 0xC5933B0A, 0xDD565F02, 0xDB3736D8, 
0x540D6F6E, 0x1402EFE8, 0x3967DEE1, 0xF4CD9815, 
0xDB1DEE76, 0xA961A227, 0x7C09FEC4, 0x86A75DF8, 
0x6D0350B4, 0xEB58EB3F, 0x0FD54601, 0x1A4DCDD7, 
0xA25720A2, 0x623E3398, 0x78423E41, 0x26169E5B, 
0xE380D3C6, 0x97AD11C9, 0x42DA5FEC, 0x25BB1ADB, 
0xC6180399, 0x011BFD4B, 0xE2032089, 0x9C94F9F1, 
0xC6851794, 0x38016FA6, 0xD2561992, 0x15399123, 
0x6850874C, 0x95F6D042, 0xA95BB38D, 0xF7127CDB, 
0x0601EC2C, 0xAE64D7B6, 0xDF6A75F3, 0xEBB621AF, 
0x5720ADCA, 0xFED1DC7C, 0xDD1BD75C, 0x489E2907, 
0xB3145380, 0x0AC7681A, 0x3AE5712F, 0x9633EB7D, 
0xB3667404, 0x4CBC02F9, 0x6F61BCE6, 0x6D1AFF77, 
0xCEAE7AAF, 0x4B2822B1, 0xF2F71F17, 0x36BECD8F, 
0x6C34ED08, 0x7F9541CA, 0x6D3F233B, 0x78A6EE2B, 
0x36B03389, 0x6199F69B, 0x27916FCA, 0xBB49D9D4, 
0x939AF6C8, 0x988EB9CD, 0xC9844B5C, 0x68200721, 
0x0B499D3E, 0x7C0AF3C8, 0xBAA15F49, 0x16D3FE7C, 
0x1766A153, 0x86764B14, 0x82600428, 0x3DCA385B, 
0x4E69A9AF, 0x6D792939, 0xAA49E182, 0x566D4D57, 
0x08CA0DAA, 0x697D05AF, 0xA9C46ECF, 0xF85393D8, 
0xDD0155DB, 0x33E868ED, 0xF7691487, 0x8B05B476, 
0x56A50ABB, 0x3263D97C, 0x2CAF7A22, 0x88EB28A9, 
0xDA2FA3C2, 0xEE66D1D4, 0xB10FF847, 0x969D55D8, 
0x0207A988, 0xD069D68D, 0x1D01175F, 0x0D83C5AC, 
0x44C59275, 0x7EE4620F, 0xD92D6DD2, 0x85340F8D, 
0x1BF1F920, 0x626E0DE1, 0x6BFA6548, 0x77299CF3, 
0x0CF233F3, 0xF4602E8C, 0x5DD29419, 0x5AEBE376, 
0x9152DA84, 0xDB624D88, 0x175B54EC, 0xB7DF8C8D, 
0x31A7653C, 0x5FECF24D, 0x3F0E5C3B, 0x1480EFA1, 
0x744A6CA2, 0x2876B663, 0x3F43F56C, 0xEB75955A, 
0xC3D24730, 0xAF77FF41, 0x8FB1B508, 0xA4351510, 
0xB5C99F8C, 0x5B784881, 0xA5C23696, 0xF529D86B, 
0xB285DC0F, 0xC4011699, 0x2BECC090, 0x38E954D2, 
0x52BF7431, 0x738AF3F2, 0x88B81A9C, 0xE5DD23CF, 
0x1ECFF19A, 0xDF9B5623, 0x45AE7CF4, 0x929DCDD8, 
0x6D3DEAA2, 0x70ABC8A6, 0xC9369E5D, 0xB990A866, 
0xC881C8E0, 0xCF24C0F1, 0xBCE7D912, 0xD24F5E11, 
0xD63201DD, 0x53BCC69D, 0x763BB4DA, 0x73425740, 
0xEFCA30F1, 0x94CC5202, 0x80A8C70C, 0x0601199D, 
0xABBFCAC4, 0x1ADBEDC8, 0x81B78A12, 0x12041E7E, 
0x738949CE, 0x5E630E67, 0xC2F066A2, 0x10C2ED6C, 
0xDEC23486, 0xC7FA2D46, 0xE9CAF225, 0x97C50FEF, 
0x55D10366, 0xFE0AD3FE, 0x61BFA712, 0x1F83EB7E, 
0x7E4D3F04, 0x5A198707, 0xCF550CF3, 0x00851993, 
0xA3AF6FC8, 0x73AFD1D9, 0x7D05994E, 0xE35201C4, 
0x7C5FEB4C, 0xC1461A0C, 0xF267C68C, 0x5F544C8A, 
0x5FF56CE7, 0xDD54E907, 0xD6D21C35, 0xAC11515D, 
0xD6F83940, 0x1E73C654, 0x91F012E1, 0x8322A9B5, 
0x68C20AD0, 0x2C092A69, 0xAC3741F7, 0x4BEFBB39, 
0x9E09371F, 0x50AF9CDF, 0x9F101FF1, 0x9CF01F32, 
0xEF264985, 0x41BC941D, 0xD0131675, 0xDFCD4D49, 
0xC3A1C8AA, 0x77DA9AAD, 0xF9A8CEDC, 0x8BCDBEF4, 
0xC3013AF5, 0x5B7F1705, 0x61668DBE, 0x3899F9BB, 
0x55C009FF, 0x7404A2BE, 0xB1D60D72, 0x6FA986F8, 
0x0464CC20, 0x5A21C430, 0x6F51A6B2, 0x8675DD51, 
0x4566FB00, 0x654EE413, 0xE67DFEE4, 0x2885674F, 
0xA24EFF17, 0x2EE39ABF, 0xCBB26F81, 0xBA50CC3A, 
0x92837FEC, 0x2C875EAB, 0x789A8011, 0xC66072CA, 
0xADBFD4D8, 0xE8A38970, 0x84ABCA1C, 0xD33BE367, 
0x5C38A483, 0xD9CFD286, 0x674EB409, 0x5A4AA698, 
0x16A75955, 0x8773A175, 0xAA1CC661, 0xC11424D7, 
0x73647AE6, 0x6A178FA5, 0xC47A88BC, 0xB233F3BA, 
0xEC17709E, 0x0B32E3BD, 0x3D037372, 0xA50D9D9A, 
0xF817E204, 0xE16D5517, 0x7E3D0E2B, 0x001B7A0E, 
0x1F0E38A1, 0x74004D49, 0x2E72C15F, 0x6DF51180, 
0xDA52FBFD, 0x3DB344BC, 0x95682565, 0x44130AA6, 
0xB08CA270, 0xC3DEC108, 0x7B78B1E6, 0x1BEDCDC9, 
0x2914B5A1, 0x7E085DA4, 0x2929EE6A, 0x5C0AC381, 
0xBD81AC0A, 0xF7BB7F19, 0x26F54249, 0x9EE39346, 
0xD55D1011, 0xA56D9FE0, 0x0B62572B, 0x5AF0B5AF, 
0x180E585F, 0x0087457F, 0x4FF98477, 0x07D99116, 
0xEF1D1D4C, 0xA0B0FA5E, 0x5A2252A6, 0x3DF6B001, 
0xD102B570, 0x0E623507, 0xB4756850, 0x55DEAA19, 
0x5655BA42, 0x91137F10, 0x067A0EFD, 0xF5FBE5B6, 
0xF69DA44B, 0xD24D4EE3, 0xA788ED05, 0x98D3EB60, 
0x3A230A03, 0x68861C16, 0x1F486B0F, 0xA30F33AE, 
0x89A043E2, 0x9B368101, 0xE7120375, 0xA0E646F9, 
0x7B79FA80, 0x13E7E35E, 0x869E5ADD, 0x2602AAD9, 
0x79398454, 0x491FDC64, 0x8424DAA0, 0xADF9DAD6, 
0x1A478BC8, 0xA458D3DA, 0x6A6BFA76, 0x9E144B58, 
0xC64A6772, 0xCC085119, 0x9FCC32A6, 0x800B77E6, 
0x15ABBEDB, 0x2AC8EDA9, 0xABB03BC9, 0xEB26F51A, 
0x80E2FA6B, 0x350F0F02, 0x07DC5C68, 0x481D4D4A, 
0x8F8792A9, 0x853720BB, 0x499B1DE9, 0x2E47D81F, 
0xA8022F1F, 0x83F6C63E, 0xEC7407E4, 0xF52D3DE1, 
0x55EA1733, 0xC6B57C01, 0x55FE91D3, 0x4557E457, 
0x2DA8F58E, 0xB6FCB7AD, 0x1E92632C, 0x874D56CA, 
0x88C43E88, 0xEB23F199, 0xBEE8C679, 0x437709D3, 
0x0FC66CC8, 0xDEE2C16F, 0xBD485130, 0xFF6C88F8, 
0x1926F6A8, 0xF6A09F75, 0x84499CEA, 0x259658A2, 
0x5F7B74BE, 0xDCF6F455, 0xBA75FF0E, 0x4C8BE368, 
0x171F6F83, 0xE73C6795, 0xB7320316, 0xECB4C0B4, 
0xFB983E7F, 0xBFFA509E, 0x14194F98, 0x8EB8681C, 
0x738F792A, 0xBCB858F7, 0x58A22C0D, 0x99E15109, 
0x055B990B, 0x770DD62A, 0xEB5421ED, 0xA6D51603, 
0x3B76258C, 0x7742629D, 0x5699E6B0, 0x2B1D1CA1, 
0x8D86A543, 0x241F75E9, 0x1007B30D, 0xA201DD4D, 
0x61F581B9, 0x07DC9586, 0xB117313E, 0x9349E07D, 
0x51494256, 0xA7213DFC, 0xA241E7D9, 0x843DCDCA, 
0xE4EA7FA1, 0x8C65E2B2, 0x7A1D4D77, 0xFF74ED9C, 
0x93729124, 0x1F322E42, 0x9102CC70, 0x5B77D78B, 
0xD5671F55, 0xE60E6822, 0x9F9AEB7B, 0x31BE130F, 
0x324381BD, 0x7C5239DB, 0xFD4B32E2, 0x08B11A8F, 
0x227C5FC4, 0x36A507E5, 0x229E3A4B, 0x58F853A4, 
0x3E8A1211, 0xAE716CB7, 0xB70B5A0F, 0x99FA66D6, 
0xEDF7410E, 0x5B4CC0DB, 0x12292AC6, 0x5431CB9D, 
0xA75A5421, 0xC690B9D7, 0xDD5212F8, 0x0033EB61, 
0xF50AC403, 0x65E3A114, 0x602CBB1C, 0x36795DC9, 
0x6E90280C, 0xC3BD203A, 0x51208CAC, 0x6D7A8A3E, 
0x7A94F8C4, 0x5588BC90, 0x2AB6FE2E, 0x0DC00939, 
0x927EADB2, 0xA5EACFD0, 0x4375872B, 0xAEC1524F, 
0x5DA5CE40, 0x3A3DE160, 0x32D7C20A, 0xB906EDEB, 
0x33C3D314, 0x7C1788B9, 0x81523455, 0xC51733A4, 
0xAD4E4597, 0xF4E13E63, 0xB76F47A2, 0x4A6CDBE1, 
0x229F9B40, 0x29427AE6, 0x4DA6825A, 0xC16D4D4B, 
0x4A5E5DA9, 0x8394B5B9, 0x9A8F6DF5, 0xC1B1F22A, 
0x8DF20338, 0xBB7D7656, 0x5681711B, 0xB2C17126, 
0x54F51677, 0x17564543, 0xE9253524, 0x1C0542A6, 
0x46DD1DEC, 0x22B7AA19, 0xCCE41197, 0x8815DE44, 
0xCC237100, 0x71180E20, 0x9644AD0E, 0x5D69AC76, 
0x6D4FC86A, 0x8EF1F80F, 0xA0DD52EF, 0x347844B5, 
0xA1C88C64, 0xC0D9E030, 0x90F9B8B2, 0x84CC3E89, 
0xF0282594, 0xB0396F49, 0xE13E35F1, 0xC5DBE369, 
0xD3F53983, 0xF499FC93, 0x18266323, 0x8F3EEADF, 
0xD1982299, 0xD6710FC6, 0x9F27B9CF, 0x4B4CAC51, 
0x72A9786E, 0x0E69214A, 0xEDD9B05E, 0x709FB068, 
0x2F80C169, 0xE3C8C986, 0x9A96EE58, 0xA6BD9E8C, 
0x7FD57703, 0xFD276F23, 0x2E04BD45, 0x460FBE35, 
0xEA0F02E5, 0xC41EAC99, 0x029DB5BC, 0xE72D990A, 
0xE9970875, 0xD1F5E750, 0xCEC75C07, 0xF17FC655, 
0xF2F5F33B, 0x9B64A8E0, 0xD81B2CCC, 0x0D9DCDBC, 
0xA0C14AA1, 0x9AD277C1, 0xCA009D84, 0x92FE07B8, 
0x7872764D, 0x46B9DD7A, 0x0B1035B7, 0x180B1BB0, 
0xD4822D99, 0x38AF3174, 0x24C18ECC, 0x086C715E, 
0x5B77AA1B, 0xD81D1C47, 0xAB9CF05D, 0xF9899208, 
0x66DAA24B, 0xBC8A146B, 0xFB0901D0, 0x63EAF548, 
0x9B038FB3, 0x5E709357, 0xA9905BBE, 0xDE062284, 
0x649AC8CA, 0x256511A5, 0x2FC9359F, 0xA367A164, 
0x4906F507, 0xBAD325BB, 0x042B58EA, 0x8983EB52, 
0xB0E09F03, 0x73403722, 0xB01F1B29, 0xD9E377D4, 
0x53911D26, 0xFA34DF52, 0xCC3DF6D2, 0x291ECE64, 
0x7ABE0707, 0xA73986E3, 0xBFED817E, 0xE47E6798, 
0xCBA2D610, 0x11A5B24C, 0xF1C74595, 0x9F99DAC9, 
0x900411C7, 0xC022FEE6, 0x0B42A99F, 0xD4088F8E, 
0x914B30B6, 0x2C16BF6A, 0x84D72604, 0xFA23FF51, 
0x24D0CB52, 0xBE1A8F3D, 0xC41F627B, 0xA992B1C8, 
0xC35B4B26, 0x0D85E5DA, 0x737FB74D, 0x4ABD4D3C, 
0x053427A9, 0x91014AC8, 0xFA72CD12, 0x532B1C45, 
0x63F3F852, 0xD204258E, 0xC09FFA42, 0x6F55B55B, 
0x541F25BA, 0x59F71E95, 0x7E5DC875, 0xF3C3A006, 
0x60114649, 0x9E729D75, 0x8B35CF02, 0x79FD56CD, 
0x0F71C387, 0x07FD1BA6, 0x6FBF7592, 0x786B4E09, 
0xCAC7350C, 0x2EF02FB0, 0x9263449D, 0x69940062, 
0x286B032F, 0x8AF2310A, 0xBDA8C38B, 0xD3021450, 
0x91E4C57A, 0xA47CDA2D, 0x27086BE4, 0x4E3BE35B, 
0xAEDCF483, 0xF2F681A1, 0x6819B33F, 0x22A805EA, 
0xC69907B3, 0x0E08BEFE, 0x093433F6, 0xF8D0E086, 
0x72C486A1, 0x500AFA8C, 0x710153AF, 0x575D1EB7, 
0x58C4EAC7, 0x4F83BCF2, 0x48E89CC3, 0x97851605, 
0xC233BA8B, 0x831C7CBA, 0xF77085D9, 0x61F150D8, 
0x47876E86, 0x741DD33A, 0xF522A66B, 0x1C3955B8, 
0x602A8F30, 0xAB2E381B, 0xDA7668DF, 0x50B5AC3C, 
0x94B29411, 0x8EA713C4, 0x0FD452CF, 0x86EDCDBD, 
0x7BA705A1, 0xA83F0CCF, 0x1AE4FC91, 0x256821C3, 
0x5E736A67, 0x7E408C92, 0x861DBFED, 0xC5AF5FF6, 
0xD3AC2CDC, 0x8A500AB6, 0xC8E9121D, 0xEF1ADFBD, 
0x74BCD278, 0x43D80FA3, 0x5AEEADC7, 0xFA510A72, 
0xB929E5C3, 0x426022F1, 0xC375E965, 0x8EDC97DB, 
0xF98CEB55, 0x0E7FCB08, 0x9B264D7D, 0x0312EE41, 
0xEC3C5E85, 0xFF8E626F, 0x3B785168, 0x029D884C, 
0xEAC3A6ED, 0x9D1690AF, 0x3AF58EDD, 0x02D3EB53, 
0x8CC75903, 0x81BDCC20, 0x00036A36, 0x7C6D92F0, 
0x3991F140, 0x12CB8E8A, 0x364B6009, 0xD6A202A9, 
0x79D9064B, 0xF9DA5F25, 0x431515DF, 0xBA2CC5E7, 
0xE5E7FE7E, 0x8C61A6A8, 0xAF18F300, 0x90714232, 
0xE462534F, 0x45070B7D, 0xE3AE7124, 0xEEFA2122, 
0xFEC4AC57, 0xCC15E61A, 0x766D27B2, 0x3E4FBB1E,

// orignal
/*
  0xd1310ba6, 0x98dfb5ac, 0x2ffd72db, 0xd01adfb7,
  0xb8e1afed, 0x6a267e96, 0xba7c9045, 0xf12c7f99,
  0x24a19947, 0xb3916cf7, 0x0801f2e2, 0x858efc16,
  0x636920d8, 0x71574e69, 0xa458fea3, 0xf4933d7e,
  0x0d95748f, 0x728eb658, 0x718bcd58, 0x82154aee,
  0x7b54a41d, 0xc25a59b5, 0x9c30d539, 0x2af26013,
  0xc5d1b023, 0x286085f0, 0xca417918, 0xb8db38ef,
  0x8e79dcb0, 0x603a180e, 0x6c9e0e8b, 0xb01e8a3e,
  0xd71577c1, 0xbd314b27, 0x78af2fda, 0x55605c60,
  0xe65525f3, 0xaa55ab94, 0x57489862, 0x63e81440,
  0x55ca396a, 0x2aab10b6, 0xb4cc5c34, 0x1141e8ce,
  0xa15486af, 0x7c72e993, 0xb3ee1411, 0x636fbc2a,
  0x2ba9c55d, 0x741831f6, 0xce5c3e16, 0x9b87931e,
  0xafd6ba33, 0x6c24cf5c, 0x7a325381, 0x28958677,
  0x3b8f4898, 0x6b4bb9af, 0xc4bfe81b, 0x66282193,
  0x61d809cc, 0xfb21a991, 0x487cac60, 0x5dec8032,
  0xef845d5d, 0xe98575b1, 0xdc262302, 0xeb651b88,
  0x23893e81, 0xd396acc5, 0x0f6d6ff3, 0x83f44239,
  0x2e0b4482, 0xa4842004, 0x69c8f04a, 0x9e1f9b5e,
  0x21c66842, 0xf6e96c9a, 0x670c9c61, 0xabd388f0,
  0x6a51a0d2, 0xd8542f68, 0x960fa728, 0xab5133a3,
  0x6eef0b6c, 0x137a3be4, 0xba3bf050, 0x7efb2a98,
  0xa1f1651d, 0x39af0176, 0x66ca593e, 0x82430e88,
  0x8cee8619, 0x456f9fb4, 0x7d84a5c3, 0x3b8b5ebe,
  0xe06f75d8, 0x85c12073, 0x401a449f, 0x56c16aa6,
  0x4ed3aa62, 0x363f7706, 0x1bfedf72, 0x429b023d,
  0x37d0d724, 0xd00a1248, 0xdb0fead3, 0x49f1c09b,
  0x075372c9, 0x80991b7b, 0x25d479d8, 0xf6e8def7,
  0xe3fe501a, 0xb6794c3b, 0x976ce0bd, 0x04c006ba,
  0xc1a94fb6, 0x409f60c4, 0x5e5c9ec2, 0x196a2463,
  0x68fb6faf, 0x3e6c53b5, 0x1339b2eb, 0x3b52ec6f,
  0x6dfc511f, 0x9b30952c, 0xcc814544, 0xaf5ebd09,
  0xbee3d004, 0xde334afd, 0x660f2807, 0x192e4bb3,
  0xc0cba857, 0x45c8740f, 0xd20b5f39, 0xb9d3fbdb,
  0x5579c0bd, 0x1a60320a, 0xd6a100c6, 0x402c7279,
  0x679f25fe, 0xfb1fa3cc, 0x8ea5e9f8, 0xdb3222f8,
  0x3c7516df, 0xfd616b15, 0x2f501ec8, 0xad0552ab,
  0x323db5fa, 0xfd238760, 0x53317b48, 0x3e00df82,
  0x9e5c57bb, 0xca6f8ca0, 0x1a87562e, 0xdf1769db,
  0xd542a8f6, 0x287effc3, 0xac6732c6, 0x8c4f5573,
  0x695b27b0, 0xbbca58c8, 0xe1ffa35d, 0xb8f011a0,
  0x10fa3d98, 0xfd2183b8, 0x4afcb56c, 0x2dd1d35b,
  0x9a53e479, 0xb6f84565, 0xd28e49bc, 0x4bfb9790,
  0xe1ddf2da, 0xa4cb7e33, 0x62fb1341, 0xcee4c6e8,
  0xef20cada, 0x36774c01, 0xd07e9efe, 0x2bf11fb4,
  0x95dbda4d, 0xae909198, 0xeaad8e71, 0x6b93d5a0,
  0xd08ed1d0, 0xafc725e0, 0x8e3c5b2f, 0x8e7594b7,
  0x8ff6e2fb, 0xf2122b64, 0x8888b812, 0x900df01c,
  0x4fad5ea0, 0x688fc31c, 0xd1cff191, 0xb3a8c1ad,
  0x2f2f2218, 0xbe0e1777, 0xea752dfe, 0x8b021fa1,
  0xe5a0cc0f, 0xb56f74e8, 0x18acf3d6, 0xce89e299,
  0xb4a84fe0, 0xfd13e0b7, 0x7cc43b81, 0xd2ada8d9,
  0x165fa266, 0x80957705, 0x93cc7314, 0x211a1477,
  0xe6ad2065, 0x77b5fa86, 0xc75442f5, 0xfb9d35cf,
  0xebcdaf0c, 0x7b3e89a0, 0xd6411bd3, 0xae1e7e49,
  0x00250e2d, 0x2071b35e, 0x226800bb, 0x57b8e0af,
  0x2464369b, 0xf009b91e, 0x5563911d, 0x59dfa6aa,
  0x78c14389, 0xd95a537f, 0x207d5ba2, 0x02e5b9c5,
  0x83260376, 0x6295cfa9, 0x11c81968, 0x4e734a41,
  0xb3472dca, 0x7b14a94a, 0x1b510052, 0x9a532915,
  0xd60f573f, 0xbc9bc6e4, 0x2b60a476, 0x81e67400,
  0x08ba6fb5, 0x571be91f, 0xf296ec6b, 0x2a0dd915,
  0xb6636521, 0xe7b9f9b6, 0xff34052e, 0xc5855664,
  0x53b02d5d, 0xa99f8fa1, 0x08ba4799, 0x6e85076a,
  0x4b7a70e9, 0xb5b32944, 0xdb75092e, 0xc4192623,
  0xad6ea6b0, 0x49a7df7d, 0x9cee60b8, 0x8fedb266,
  0xecaa8c71, 0x699a17ff, 0x5664526c, 0xc2b19ee1,
  0x193602a5, 0x75094c29, 0xa0591340, 0xe4183a3e,
  0x3f54989a, 0x5b429d65, 0x6b8fe4d6, 0x99f73fd6,
  0xa1d29c07, 0xefe830f5, 0x4d2d38e6, 0xf0255dc1,
  0x4cdd2086, 0x8470eb26, 0x6382e9c6, 0x021ecc5e,
  0x09686b3f, 0x3ebaefc9, 0x3c971814, 0x6b6a70a1,
  0x687f3584, 0x52a0e286, 0xb79c5305, 0xaa500737,
  0x3e07841c, 0x7fdeae5c, 0x8e7d44ec, 0x5716f2b8,
  0xb03ada37, 0xf0500c0d, 0xf01c1f04, 0x0200b3ff,
  0xae0cf51a, 0x3cb574b2, 0x25837a58, 0xdc0921bd,
  0xd19113f9, 0x7ca92ff6, 0x94324773, 0x22f54701,
  0x3ae5e581, 0x37c2dadc, 0xc8b57634, 0x9af3dda7,
  0xa9446146, 0x0fd0030e, 0xecc8c73e, 0xa4751e41,
  0xe238cd99, 0x3bea0e2f, 0x3280bba1, 0x183eb331,
  0x4e548b38, 0x4f6db908, 0x6f420d03, 0xf60a04bf,
  0x2cb81290, 0x24977c79, 0x5679b072, 0xbcaf89af,
  0xde9a771f, 0xd9930810, 0xb38bae12, 0xdccf3f2e,
  0x5512721f, 0x2e6b7124, 0x501adde6, 0x9f84cd87,
  0x7a584718, 0x7408da17, 0xbc9f9abc, 0xe94b7d8c,
  0xec7aec3a, 0xdb851dfa, 0x63094366, 0xc464c3d2,
  0xef1c1847, 0x3215d908, 0xdd433b37, 0x24c2ba16,
  0x12a14d43, 0x2a65c451, 0x50940002, 0x133ae4dd,
  0x71dff89e, 0x10314e55, 0x81ac77d6, 0x5f11199b,
  0x043556f1, 0xd7a3c76b, 0x3c11183b, 0x5924a509,
  0xf28fe6ed, 0x97f1fbfa, 0x9ebabf2c, 0x1e153c6e,
  0x86e34570, 0xeae96fb1, 0x860e5e0a, 0x5a3e2ab3,
  0x771fe71c, 0x4e3d06fa, 0x2965dcb9, 0x99e71d0f,
  0x803e89d6, 0x5266c825, 0x2e4cc978, 0x9c10b36a,
  0xc6150eba, 0x94e2ea78, 0xa5fc3c53, 0x1e0a2df4,
  0xf2f74ea7, 0x361d2b3d, 0x1939260f, 0x19c27960,
  0x5223a708, 0xf71312b6, 0xebadfe6e, 0xeac31f66,
  0xe3bc4595, 0xa67bc883, 0xb17f37d1, 0x018cff28,
  0xc332ddef, 0xbe6c5aa5, 0x65582185, 0x68ab9802,
  0xeecea50f, 0xdb2f953b, 0x2aef7dad, 0x5b6e2f84,
  0x1521b628, 0x29076170, 0xecdd4775, 0x619f1510,
  0x13cca830, 0xeb61bd96, 0x0334fe1e, 0xaa0363cf,
  0xb5735c90, 0x4c70a239, 0xd59e9e0b, 0xcbaade14,
  0xeecc86bc, 0x60622ca7, 0x9cab5cab, 0xb2f3846e,
  0x648b1eaf, 0x19bdf0ca, 0xa02369b9, 0x655abb50,
  0x40685a32, 0x3c2ab4b3, 0x319ee9d5, 0xc021b8f7,
  0x9b540b19, 0x875fa099, 0x95f7997e, 0x623d7da8,
  0xf837889a, 0x97e32d77, 0x11ed935f, 0x16681281,
  0x0e358829, 0xc7e61fd6, 0x96dedfa1, 0x7858ba99,
  0x57f584a5, 0x1b227263, 0x9b83c3ff, 0x1ac24696,
  0xcdb30aeb, 0x532e3054, 0x8fd948e4, 0x6dbc3128,
  0x58ebf2ef, 0x34c6ffea, 0xfe28ed61, 0xee7c3c73,
  0x5d4a14d9, 0xe864b7e3, 0x42105d14, 0x203e13e0,
  0x45eee2b6, 0xa3aaabea, 0xdb6c4f15, 0xfacb4fd0,
  0xc742f442, 0xef6abbb5, 0x654f3b1d, 0x41cd2105,
  0xd81e799e, 0x86854dc7, 0xe44b476a, 0x3d816250,
  0xcf62a1f2, 0x5b8d2646, 0xfc8883a0, 0xc1c7b6a3,
  0x7f1524c3, 0x69cb7492, 0x47848a0b, 0x5692b285,
  0x095bbf00, 0xad19489d, 0x1462b174, 0x23820e00,
  0x58428d2a, 0x0c55f5ea, 0x1dadf43e, 0x233f7061,
  0x3372f092, 0x8d937e41, 0xd65fecf1, 0x6c223bdb,
  0x7cde3759, 0xcbee7460, 0x4085f2a7, 0xce77326e,
  0xa6078084, 0x19f8509e, 0xe8efd855, 0x61d99735,
  0xa969a7aa, 0xc50c06c2, 0x5a04abfc, 0x800bcadc,
  0x9e447a2e, 0xc3453484, 0xfdd56705, 0x0e1e9ec9,
  0xdb73dbd3, 0x105588cd, 0x675fda79, 0xe3674340,
  0xc5c43465, 0x713e38d8, 0x3d28f89e, 0xf16dff20,
  0x153e21e7, 0x8fb03d4a, 0xe6e39f2b, 0xdb83adf7,
  0xe93d5a68, 0x948140f7, 0xf64c261c, 0x94692934,
  0x411520f7, 0x7602d4f7, 0xbcf46b2e, 0xd4a20068,
  0xd4082471, 0x3320f46a, 0x43b7d4b7, 0x500061af,
  0x1e39f62e, 0x97244546, 0x14214f74, 0xbf8b8840,
  0x4d95fc1d, 0x96b591af, 0x70f4ddd3, 0x66a02f45,
  0xbfbc09ec, 0x03bd9785, 0x7fac6dd0, 0x31cb8504,
  0x96eb27b3, 0x55fd3941, 0xda2547e6, 0xabca0a9a,
  0x28507825, 0x530429f4, 0x0a2c86da, 0xe9b66dfb,
  0x68dc1462, 0xd7486900, 0x680ec0a4, 0x27a18dee,
  0x4f3ffea2, 0xe887ad8c, 0xb58ce006, 0x7af4d6b6,
  0xaace1e7c, 0xd3375fec, 0xce78a399, 0x406b2a42,
  0x20fe9e35, 0xd9f385b9, 0xee39d7ab, 0x3b124e8b,
  0x1dc9faf7, 0x4b6d1856, 0x26a36631, 0xeae397b2,
  0x3a6efa74, 0xdd5b4332, 0x6841e7f7, 0xca7820fb,
  0xfb0af54e, 0xd8feb397, 0x454056ac, 0xba489527,
  0x55533a3a, 0x20838d87, 0xfe6ba9b7, 0xd096954b,
  0x55a867bc, 0xa1159a58, 0xcca92963, 0x99e1db33,
  0xa62a4a56, 0x3f3125f9, 0x5ef47e1c, 0x9029317c,
  0xfdf8e802, 0x04272f70, 0x80bb155c, 0x05282ce3,
  0x95c11548, 0xe4c66d22, 0x48c1133f, 0xc70f86dc,
  0x07f9c9ee, 0x41041f0f, 0x404779a4, 0x5d886e17,
  0x325f51eb, 0xd59bc0d1, 0xf2bcc18f, 0x41113564,
  0x257b7834, 0x602a9c60, 0xdff8e8a3, 0x1f636c1b,
  0x0e12b4c2, 0x02e1329e, 0xaf664fd1, 0xcad18115,
  0x6b2395e0, 0x333e92e1, 0x3b240b62, 0xeebeb922,
  0x85b2a20e, 0xe6ba0d99, 0xde720c8c, 0x2da2f728,
  0xd0127845, 0x95b794fd, 0x647d0862, 0xe7ccf5f0,
  0x5449a36f, 0x877d48fa, 0xc39dfd27, 0xf33e8d1e,
  0x0a476341, 0x992eff74, 0x3a6f6eab, 0xf4f8fd37,
  0xa812dc60, 0xa1ebddf8, 0x991be14c, 0xdb6e6b0d,
  0xc67b5510, 0x6d672c37, 0x2765d43b, 0xdcd0e804,
  0xf1290dc7, 0xcc00ffa3, 0xb5390f92, 0x690fed0b,
  0x667b9ffb, 0xcedb7d9c, 0xa091cf0b, 0xd9155ea3,
  0xbb132f88, 0x515bad24, 0x7b9479bf, 0x763bd6eb,
  0x37392eb3, 0xcc115979, 0x8026e297, 0xf42e312d,
  0x6842ada7, 0xc66a2b3b, 0x12754ccc, 0x782ef11c,
  0x6a124237, 0xb79251e7, 0x06a1bbe6, 0x4bfb6350,
  0x1a6b1018, 0x11caedfa, 0x3d25bdd8, 0xe2e1c3c9,
  0x44421659, 0x0a121386, 0xd90cec6e, 0xd5abea2a,
  0x64af674e, 0xda86a85f, 0xbebfe988, 0x64e4c3fe,
  0x9dbc8057, 0xf0f7c086, 0x60787bf8, 0x6003604d,
  0xd1fd8346, 0xf6381fb0, 0x7745ae04, 0xd736fccc,
  0x83426b33, 0xf01eab71, 0xb0804187, 0x3c005e5f,
  0x77a057be, 0xbde8ae24, 0x55464299, 0xbf582e61,
  0x4e58f48f, 0xf2ddfda2, 0xf474ef38, 0x8789bdc2,
  0x5366f9c3, 0xc8b38e74, 0xb475f255, 0x46fcd9b9,
  0x7aeb2661, 0x8b1ddf84, 0x846a0e79, 0x915f95e2,
  0x466e598e, 0x20b45770, 0x8cd55591, 0xc902de4c,
  0xb90bace1, 0xbb8205d0, 0x11a86248, 0x7574a99e,
  0xb77f19b6, 0xe0a9dc09, 0x662d09a1, 0xc4324633,
  0xe85a1f02, 0x09f0be8c, 0x4a99a025, 0x1d6efe10,
  0x1ab93d1d, 0x0ba5a4df, 0xa186f20f, 0x2868f169,
  0xdcb7da83, 0x573906fe, 0xa1e2ce9b, 0x4fcd7f52,
  0x50115e01, 0xa70683fa, 0xa002b5c4, 0x0de6d027,
  0x9af88c27, 0x773f8641, 0xc3604c06, 0x61a806b5,
  0xf0177a28, 0xc0f586e0, 0x006058aa, 0x30dc7d62,
  0x11e69ed7, 0x2338ea63, 0x53c2dd94, 0xc2c21634,
  0xbbcbee56, 0x90bcb6de, 0xebfc7da1, 0xce591d76,
  0x6f05e409, 0x4b7c0188, 0x39720a3d, 0x7c927c24,
  0x86e3725f, 0x724d9db9, 0x1ac15bb4, 0xd39eb8fc,
  0xed545578, 0x08fca5b5, 0xd83d7cd3, 0x4dad0fc4,
  0x1e50ef5e, 0xb161e6f8, 0xa28514d9, 0x6c51133c,
  0x6fd5c7e7, 0x56e14ec4, 0x362abfce, 0xddc6c837,
  0xd79a3234, 0x92638212, 0x670efa8e, 0x406000e0,
  0x3a39ce37, 0xd3faf5cf, 0xabc27737, 0x5ac52d1b,
  0x5cb0679e, 0x4fa33742, 0xd3822740, 0x99bc9bbe,
  0xd5118e9d, 0xbf0f7315, 0xd62d1c7e, 0xc700c47b,
  0xb78c1b6b, 0x21a19045, 0xb26eb1be, 0x6a366eb4,
  0x5748ab2f, 0xbc946e79, 0xc6a376d2, 0x6549c2c8,
  0x530ff8ee, 0x468dde7d, 0xd5730a1d, 0x4cd04dc6,
  0x2939bbdb, 0xa9ba4650, 0xac9526e8, 0xbe5ee304,
  0xa1fad5f0, 0x6a2d519a, 0x63ef8ce2, 0x9a86ee22,
  0xc089c2b8, 0x43242ef6, 0xa51e03aa, 0x9cf2d0a4,
  0x83c061ba, 0x9be96a4d, 0x8fe51550, 0xba645bd6,
  0x2826a2f9, 0xa73a3ae1, 0x4ba99586, 0xef5562e9,
  0xc72fefd3, 0xf752f7da, 0x3f046f69, 0x77fa0a59,
  0x80e4a915, 0x87b08601, 0x9b09e6ad, 0x3b3ee593,
  0xe990fd5a, 0x9e34d797, 0x2cf0b7d9, 0x022b8b51,
  0x96d5ac3a, 0x017da67d, 0xd1cf3ed6, 0x7c7d2d28,
  0x1f9f25cf, 0xadf2b89b, 0x5ad6b472, 0x5a88f54c,
  0xe029ac71, 0xe019a5e6, 0x47b0acfd, 0xed93fa9b,
  0xe8d3c48d, 0x283b57cc, 0xf8d56629, 0x79132e28,
  0x785f0191, 0xed756055, 0xf7960e44, 0xe3d35e8c,
  0x15056dd4, 0x88f46dba, 0x03a16125, 0x0564f0bd,
  0xc3eb9e15, 0x3c9057a2, 0x97271aec, 0xa93a072a,
  0x1b3f6d9b, 0x1e6321f5, 0xf59c66fb, 0x26dcf319,
  0x7533d928, 0xb155fdf5, 0x03563482, 0x8aba3cbb,
  0x28517711, 0xc20ad9f8, 0xabcc5167, 0xccad925f,
  0x4de81751, 0x3830dc8e, 0x379d5862, 0x9320f991,
  0xea7a90c2, 0xfb3e7bce, 0x5121ce64, 0x774fbe32,
  0xa8b6e37e, 0xc3293d46, 0x48de5369, 0x6413e680,
  0xa2ae0810, 0xdd6db224, 0x69852dfd, 0x09072166,
  0xb39a460a, 0x6445c0dd, 0x586cdecf, 0x1c20c8ae,
  0x5bbef7dd, 0x1b588d40, 0xccd2017f, 0x6bb4e3bb,
  0xdda26a7e, 0x3a59ff45, 0x3e350a44, 0xbcb4cdd5,
  0x72eacea8, 0xfa6484bb, 0x8d6612ae, 0xbf3c6f47,
  0xd29be463, 0x542f5d9e, 0xaec2771b, 0xf64e6370,
  0x740e0d8d, 0xe75b1357, 0xf8721671, 0xaf537d5d,
  0x4040cb08, 0x4eb4e2cc, 0x34d2466a, 0x0115af84,
  0xe1b00428, 0x95983a1d, 0x06b89fb4, 0xce6ea048,
  0x6f3f3b82, 0x3520ab82, 0x011a1d4b, 0x277227f8,
  0x611560b1, 0xe7933fdc, 0xbb3a792b, 0x344525bd,
  0xa08839e1, 0x51ce794b, 0x2f32c9b7, 0xa01fbac9,
  0xe01cc87e, 0xbcc7d1f6, 0xcf0111c3, 0xa1e8aac7,
  0x1a908749, 0xd44fbd9a, 0xd0dadecb, 0xd50ada38,
  0x0339c32a, 0xc6913667, 0x8df9317c, 0xe0b12b4f,
  0xf79e59b7, 0x43f5bb3a, 0xf2d519ff, 0x27d9459c,
  0xbf97222c, 0x15e6fc2a, 0x0f91fc71, 0x9b941525,
  0xfae59361, 0xceb69ceb, 0xc2a86459, 0x12baa8d1,
  0xb6c1075e, 0xe3056a0c, 0x10d25065, 0xcb03a442,
  0xe0ec6e0e, 0x1698db3b, 0x4c98a0be, 0x3278e964,
  0x9f1f9532, 0xe0d392df, 0xd3a0342b, 0x8971f21e,
  0x1b0a7441, 0x4ba3348c, 0xc5be7120, 0xc37632d8,
  0xdf359f8d, 0x9b992f2e, 0xe60b6f47, 0x0fe3f11d,
  0xe54cda54, 0x1edad891, 0xce6279cf, 0xcd3e7e6f,
  0x1618b166, 0xfd2c1d05, 0x848fd2c5, 0xf6fb2299,
  0xf523f357, 0xa6327623, 0x93a83531, 0x56cccd02,
  0xacf08162, 0x5a75ebb5, 0x6e163697, 0x88d273cc,
  0xde966292, 0x81b949d0, 0x4c50901b, 0x71c65614,
  0xe6c6c7bd, 0x327a140a, 0x45e1d006, 0xc3f27b9a,
  0xc9aa53fd, 0x62a80f00, 0xbb25bfe2, 0x35bdd2f6,
  0x71126905, 0xb2040222, 0xb6cbcf7c, 0xcd769c2b,
  0x53113ec0, 0x1640e3d3, 0x38abbd60, 0x2547adf0,
  0xba38209c, 0xf746ce76, 0x77afa1c5, 0x20756060,
  0x85cbfe4e, 0x8ae88dd8, 0x7aaaf9b0, 0x4cf9aa7e,
  0x1948c25c, 0x02fb8a8c, 0x01c36ae4, 0xd6ebe1f9,
  0x90d4f869, 0xa65cdea0, 0x3f09252d, 0xc208e69f,
  0xb74e6132, 0xce77e25b, 0x578fdfe3, 0x3ac372e6,
  */
};


