#include "myspnplus.h"

MySPNPlus::MySPNPlus()
{
    unsigned char thissbox[256] = {

        //0 1 2 3 4 5 6 7 8 9 A B C D E F

        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0

        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1

        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2

        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3

        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4

        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5

        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6

        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7

        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8

        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9

        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A

        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B

        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C

        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D

        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E

        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }; //F

    unsigned char thisrsbox[256] = {

        //0 1 2 3 4 5 6 7 8 9 A B C D E F

        0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,

        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,

        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,

        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,

        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4,

        0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,

        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,

        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,

        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,

        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,

        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,

        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,

        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,

        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,

        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,

        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,

        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
    for (int i = 0; i < 256; i++)
    {
        sbox[i] = thissbox[i];
        sboxDecrypt[i] = thisrsbox[i];
    }
}

MySPNPlus::~MySPNPlus()
{
}

void MySPNPlus::setKey(unsigned char * key)
{
    unsigned char temp[32];
    for (int i = 0; i < 16; ++i) {
        temp[2 * i] = key[i] & 0x0f;
        temp[2 * i + 1] = (key[i] >> 4) & 0x0f;
    }
    for (int i = 0; i < 17; ++i)
    {
        char tempthis[16];
        for (int j = 0; j<8; ++j)
        {
            tempthis[j] = temp[i + j*2] | ((temp[i+j*2+1] << 4) & 0xf0);
        }
        roundkey[i] = *(unsigned long long *)tempthis;
    }
    return;
}

void MySPNPlus::setKeyDecrypt(unsigned char * key)
{
    unsigned char temp[32];
    for (int i = 0; i < 16; ++i) {
        temp[2 * i] = key[i] & 0x0f;
        temp[2 * i + 1] = (key[i] >> 4) & 0x0f;
    }
    for (int i = 0; i < 17; ++i)
    {
        char tempthis[8];
        for (int j = 0; j<8; ++j)
        {
            tempthis[j] = temp[i + j * 2] | ((temp[i + j * 2 + 1] << 4) & 0xf0);
        }
        roundkeyDecrypt[i] = *(unsigned long long *)tempthis;
    }
}


unsigned long long MySPNPlus::encrypt(unsigned long long plaintext)
{
    unsigned long long crytext=plaintext;
    //unsigned char * temp = (unsigned char *)&crytext;

    for (int i = 0; i < 15; i++)
    {
        crytext = crytext^roundkey[i];
        sbox64((unsigned char *)&crytext);
        crytext=pbox64((unsigned char *)&crytext);
    }
    crytext = crytext^roundkey[15];
    sbox64((unsigned char *)&crytext);
    crytext = crytext^roundkey[16];
    return crytext;
}

bool MySPNPlus::encryptFile(char * filename, char * newfilename)
{
    FILE * file;
    FILE * aimfile;
    if (fopen_s(&file, filename, "rb")) {
        //std::cout << filename << "´ò¿ªÊ§°Ü" << std::endl;
        return false;
    }
    if (fopen_s(&aimfile, newfilename, "wb")) {
        //std::cout << newfilename << "´´½¨Ê§°Ü" << std::endl;
        return false;
    }
    unsigned long long temp  = 0x123456789abcdef0;
    char c1[8];
    bool filestate = true;
    while (filestate) {
        for (int i = 0; i < 8; i++)
        {
            c1[i] = fgetc(file);
            if (feof(file)) {
                switch (i)
                {
                case 0:c1[0] = 0; c1[1] = 0; c1[2] = 0; c1[3] = 0; c1[4] = 0; c1[5] = 0; c1[6] = 0; c1[7] = 8; break;
                case 1:c1[1] = 0; c1[2] = 0; c1[3] = 0; c1[4] = 0; c1[5] = 0; c1[6] = 0; c1[7] = 7; break;
                case 2:c1[2] = 0; c1[3] = 0; c1[4] = 0; c1[5] = 0; c1[6] = 0; c1[7] = 6; break;
                case 3:c1[3] = 0; c1[4] = 0; c1[5] = 0; c1[6] = 0; c1[7] = 5; break;
                case 4:c1[4] = 0; c1[5] = 0; c1[6] = 0; c1[7] = 4; break;
                case 5:c1[5] = 0; c1[6] = 0; c1[7] = 3; break;
                case 6:c1[6] = 0; c1[7] = 2; break;
                case 7:c1[7] = 1; break;
                default:
                    break;
                }
                filestate = false;
            }
        }
        unsigned long long y = encrypt(((*((unsigned long long *)c1)))^temp);
        char * out = (char *)&y;
        temp = y;
        fputc(out[0], aimfile);
        fputc(out[1], aimfile);
        fputc(out[2], aimfile);
        fputc(out[3], aimfile);
        fputc(out[4], aimfile);
        fputc(out[5], aimfile);
        fputc(out[6], aimfile);
        fputc(out[7], aimfile);
    }
    fclose(file);
    fclose(aimfile);
    return true;
}

unsigned long long MySPNPlus::decrypt(unsigned long long crytext1)
{
    unsigned long long crytext = crytext1;
    //unsigned char * temp = (unsigned char *)&crytext;
    crytext = crytext^roundkeyDecrypt[16];
    sboxDecrypt64((unsigned char *)&crytext);
    crytext = crytext^roundkeyDecrypt[15];
    for (int i = 14; i >= 0; i--)
    {
        crytext = pbox64((unsigned char *)&crytext);
        sboxDecrypt64((unsigned char *)&crytext);
        crytext = crytext^roundkeyDecrypt[i];
    }
    return crytext;
}

bool MySPNPlus::decryptFile(char * filename, char * newfilename)
{
    FILE * file;
    FILE * aimfile;
    if (fopen_s(&file, filename, "rb")) {
        //std::cout << filename << "´ò¿ªÊ§°Ü" << std::endl;
        return false;
    }
    if (fopen_s(&aimfile, newfilename, "wb")) {
        //std::cout << newfilename << "´´½¨Ê§°Ü" << std::endl;
        return false;
    }
    //int i = 0;
    char c1[8];
    unsigned long long temp = 0x123456789abcdef0;
    c1[0] = fgetc(file);
    while (!feof(file)) {
        //i++;
        c1[1] = fgetc(file);
        c1[2] = fgetc(file);
        c1[3] = fgetc(file);
        c1[4] = fgetc(file);
        c1[5] = fgetc(file);
        c1[6] = fgetc(file);
        c1[7] = fgetc(file);

        unsigned long long y;
        y = decrypt((unsigned long long)*((unsigned long long *)c1));
        y = y^temp;
        temp = (unsigned long long )*((unsigned long long *)c1);
        char * out = (char *)&y;
        c1[0] = fgetc(file);
        if (feof(file)) {
            switch (c1[3])
            {
            case 0:break;
            case 1:fputc(out[0], aimfile); fputc(out[1], aimfile); fputc(out[2], aimfile); fputc(out[3], aimfile); fputc(out[4], aimfile); fputc(out[5], aimfile); fputc(out[6], aimfile); break;
            case 2:fputc(out[0], aimfile); fputc(out[1], aimfile); fputc(out[2], aimfile); fputc(out[3], aimfile); fputc(out[4], aimfile); fputc(out[5], aimfile); break;
            case 3:fputc(out[0], aimfile); fputc(out[1], aimfile); fputc(out[2], aimfile); fputc(out[3], aimfile); fputc(out[4], aimfile); break;
            case 4:fputc(out[0], aimfile); fputc(out[1], aimfile); fputc(out[2], aimfile); fputc(out[3], aimfile); break;
            case 5:fputc(out[0], aimfile); fputc(out[1], aimfile); fputc(out[2], aimfile); break;
            case 6:fputc(out[0], aimfile); fputc(out[1], aimfile); break;
            case 7:fputc(out[0], aimfile); break;
            default:
                break;
            }
            break;
        }
        fputc(out[0], aimfile);
        fputc(out[1], aimfile);
        fputc(out[2], aimfile);
        fputc(out[3], aimfile);
        fputc(out[4], aimfile);
        fputc(out[5], aimfile);
        fputc(out[6], aimfile);
        fputc(out[7], aimfile);
    }
    //std::cout << i << std::endl;
    fclose(file);
    fclose(aimfile);
    return true;
}

inline unsigned long long MySPNPlus::pbox64(unsigned char * tempp)
{
    unsigned long long x = *(unsigned long long*)tempp;
    char temp[64];
    char aim[64];
    for (int i = 0; i < 64; i++)
    {
        temp[i] = x % 2;
        x /= 2;
    }
    for (int i = 0; i < 8; i++)
    {
        aim[i] = temp[i*8];
        aim[i+8] = temp[i*8+1];
        aim[i+16] = temp[i*8+2];
        aim[i+24] = temp[i*8+3];
        aim[i+32] = temp[i*8+4];
        aim[i+40] = temp[i*8+5];
        aim[i+48] = temp[i*8+6];
        aim[i+56] = temp[i*8+7];
    }
    unsigned long long result=0,y=1;
    for (int i = 0; i < 64; i++)
    {
        result = aim[i] * y+result;
        y *= 2;
    }
    return result;
}

void MySPNPlus::getKey(QString name, int state){
    unsigned char key[16];
    for(int i=0;i<16;++i){
        key[i]=((unsigned char)i)|name.at(i%name.length()).toLatin1();
    }
    if(state==1){
        setKey(key);
    }else if(state==-1){
        setKeyDecrypt(key);
    }else{
        setKey(key);
        setKeyDecrypt(key);
    }
}

void MySPNPlus::encrypt(QString input, QString *output){
    char text[500];
    strcpy(text,input.toStdString().c_str());
    int length=strlen(text);
    int num =length/8;
    if(length%8){
        num++;
        strcat(text,"        ");
    }
    //qDebug()<<"input = "<<input<<length<<num;
    for(int i=0;i<num;i++){
        *((unsigned long long*)(text)+i)=encrypt(*((unsigned long long*)(text)+i));
    }
    text[num*8]='\0';
    output->clear();
    *output=QByteArray(text).toHex();
}

void MySPNPlus::decrypt(QString input, QString *output){
    char text[500];
    strcpy(text,QByteArray::fromHex(input.toLatin1()).data());
    int length=strlen(text);
    int num=length/8;
    for(int i=0;i<num;i++){
        *((unsigned long long*)(text)+i)=decrypt(*((unsigned long long*)(text)+i));
    }
    output->clear();
    *output=QString(text);
}
