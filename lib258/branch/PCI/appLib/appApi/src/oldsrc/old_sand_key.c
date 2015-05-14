/*
��������˲��ּ��ܺ�������
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termio.h>
#include <termios.h>    /*PPSIX�ն˿��ƶ���*/
#include <errno.h>      /*����Ŷ���*/
#include <time.h>       /*ʱ�亯��*/
#include <sys/time.h>
#include <getopt.h>
#include <signal.h>


#define __LUXAPP_KBD__
#include "sand_key.h"

#ifdef EN_DBG
#define dbg Uart_Printf
#else
#define dbg
#endif

char *KEY_DEVICE="/dev/ttyS1";
static int fd_comkey=0;
DRV_OUT *kbd=NULL;


/*
 *�������������������ȴ�Ӧ��ȡ��ֵ
 * ���ض����ĳ��ȣ�
 *
 */
int get_key_from_serial(char *dev, unsigned char *data, unsigned char *status)
{
    int fd, ret, i;
    int count=100;


    return ret;
}



unsigned char scancode_to_asc(unsigned char in)
{

#if 0
    unsigned char out;
    dbg("in=0x%x ",  in);
    switch (in)
    {
    case SKBKEY_ENTER1:
    case SKBKEY_ENTER2:
        out=0x0d;
        break;
    case SKBKEY_N0: 	//����0
        out= '0';
        break;
    case SKBKEY_N1: 	//���� 1
        out= '1';
        break;
    case SKBKEY_N2: 	//���� 2
        out= '2';
        break;
    case SKBKEY_N3: 	//���� 3
        out= '3';
        break;
    case SKBKEY_N4: 	//���� 4
        out= '4';
        break;
    case SKBKEY_N5: 	//���� 5
        out= '5';
        break;
    case SKBKEY_N6: 	//���� 6
        out= '6';
        break;
    case SKBKEY_N7: 	//���� 7
        out= '7';
        break;
    case SKBKEY_N8: 	//���� 8
        out= '8';
        break;
    case SKBKEY_N9: 	//���� 9
        out= '9';
        break;

    case SKBKEY_N00: 	//���� �� .
        out= KEY_00_PT;
        break;


    case SKBKEY_F1:
        out=KEY_F1;
        break;
    case SKBKEY_F2:
        out=KEY_F2;
        break;
    case SKBKEY_F3:
        out=KEY_F3;
        break;
    case SKBKEY_F4:
        out=KEY_F4;
        break;

    case SKBKEY_CANCEL: 	//ȡ����,esc?
        out= KEY_CANCEL;
        break;

    case SKBKEY_CLEAR: 	//�����,backspace?
        out= KEY_BCKSP;
        break;

    case SKBKEY_SUBTOTAL:	// С��
        out=KEY_SUBTOTAL;
        break;

    case SKBKEY_PAPER_FEED: 	//���ڽ�ֽ��
        out= KEY_PAPER_FEED;
        break;

    case SKBKEY_PAPER_REVERSEFEED: 	//���ڽ�ֽ��
        out= KEY_PAPER_REVERSEFEED;
        break;

    case SKBKEY_CASHIER:		//ӪҵԱ
        out=CASHKEY_CASHIER;
        break;

    case SKBKEY_JUMP:			//jump out
        out=KEY_JUMP;
        break;

    case SKBKEY_ADDING:
        out=KEY_ADDING;
        break;
    case SKBKEY_DECREASE:
        out=KEY_DECREASE;
        break;
    case SKBKEY_MUITIPLY:
        out=KEY_MUITIPLY;
        break;
    case SKBKEY_DIVISION:
        out=KEY_DIVISION;
        break;


        //��ĸ��
    case SKBKEY_A: 	//��ĸa
        out= 'A';
        break;

    case SKBKEY_B: 	//��ĸ b
        out= 'B';
        break;

    case SKBKEY_C: 	//��ĸ c
        out= 'C';
        break;

    case SKBKEY_D: 	//��ĸ d
        out= 'D';
        break;

    case SKBKEY_E: 	//��ĸ e
        out= 'E';
        break;

    case SKBKEY_F: 	//��ĸ f
        out= 'F';
        break;

    case SKBKEY_G: 	//��ĸ g
        out= 'G';
        break;

    case SKBKEY_H: 	//��ĸ h
        out= 'H';
        break;

    case SKBKEY_I: 	//��ĸ i
        out= 'I';
        break;

    case SKBKEY_J: 	//��ĸ j
        out= 'J';
        break;

    case SKBKEY_K: 	//��ĸ k
        out= 'K';
        break;

    case SKBKEY_L: 	//��ĸ l
        out= 'L';
        break;

    case SKBKEY_M: 	//��ĸ m
        out= 'M';
        break;

    case SKBKEY_N: 	//��ĸ n
        out= 'N';
        break;

    case SKBKEY_O: 	//��ĸ o
        out= 'O';
        break;

    case SKBKEY_P: 	//��ĸ p		//?
        out= 'P';
        break;

    case SKBKEY_Q: 	//��ĸ q
        out= 'Q';
        break;

    case SKBKEY_R: 	//��ĸ r
        out= 'R';
        break;

    case SKBKEY_S: 	//��ĸ s
        out= 'S';
        break;

    case SKBKEY_T: 	//��ĸ t
        out= 'T';
        break;

    case SH�=Q�  ��p@ H���o@ �w���H�D$h    H�D$0�4���H�T$`H�|$0E1�E1�H���Ht@ H�T$H�T$h1�H�D$    H�$@   ��������   E���  H����@ 1��:���f.�     � ���V�����H�D$X���@ H�H����A�   ����H�=φ  H���  �!)  H��H��A�   �����H�=p�  ��p@ H�¾�o@ E0�����H�T$`E1�1�H�Ǿ�t@ 1�H�D$`    ��������B���H�D$`��t@ A�H�p1�����H�|$`���������H�D$h�\t@ H�p1�A�   �W���H�|$h����H�|$`����������H�D$pE1�E1�1�1��   H�D$H�D$|L��H�D$p    H�D$    H�D$H�$    ��������	  �D$|A�}   �u���A��뎃=��   �	  H�=��  H���
  ��'  H��H��������L$LH�D$@H�D$`    D�a�L�hE��~H�P�:-u�z-u�z uL�hD�a�H�=�  ��p@ H���o@ �(���D���  H�D$h    H�D$0D�D$8�����D�D$8I��H�|$0H�D$`H�T$h�   ��t@ H�D$     H�D$@   H�D$1�H�D$    H�$    �)������P  E���������  H��@ 1������ � ���������=��   �/  �=��   ��  H�=��  H����   �&  H��H��A�   ����H�=d�  E1�L�-b�  H��t1Ҿ/u@ ����I��H�=߃  ��p@ H���o@ ����H�D$h    H�D$0�����H�|$0I��H�D$pH�T$hM��@   H�D$�1u@ 1�H�D$     H�D$@   H�D$    L�$$�������x  H�T$p�Su@ H��1�����H�|$p�B���L��E1��w����K���H�=��  H����   �%  H��H��A�   �$���H�=��  E1�H��t1Ҿ/u@ � ���I��H�=�  ��p@ H���o@ ����H�D$h    I�������H�T$hE1�H��M��1��eu@ L��H�$    �N������T���H�D$h�wu@ H�p1������H�|$h�7����0���H�=�  H����   ��$  H��H��A�   �d���L�%Ԃ  E1�M��t1Ҿ/u@ L���]���I��H�=+�  ��p@ H���o@ �Q���H�D$h    H�D$0����H�|$0H�T$hE1�H��M��1���u@ H�$    ������uH�D$h��u@ H�p1��.���H�|$h�t���L��E1����������H�=5�  H����   �/$  H��H��A�   �����H�=�  E1�H��t1Ҿ/u@ ����I��H�=`�  ��p@ H���o@ ����H�D$h    I���E���H�T$hE1�H��M��1���u@ L��H�$    ������������H�D$h��u@ H�p1��c���H�|$h��������H�=��  H����   �o#  H��H��A�   �����D�5n�  L�-_�  �:���I��E��u��u@ �����L��H�������M��tL���u@ 1������L��H�������1�L�������H�=e�  ��p@ H���o@ ����H�D$h    I���J���H�$    M�$H�T$hE1�H����u@ 1�L���������t_H���H&  L��1ҾP@ ����L��   E1��I��������1�1�H���X�����r@ H��H��1�A�   �P���H��1����������H�D$h�H�@ H�p1�����H�|$h�R���눋L$LH�D$@H�D$`    �i�L�`��~H�P�:-u�z-u�z uL�`�i�H�=X  ��o@ ��o@ H���~���L�L$`H�T$h1�H��A�@   1��'u@ H�D$h    H�$    ��������   ��u<��@ 1��d���� ��������H�D$h�Au@ H�p1��C���H�|$h��������H�D$pE1�E1�1�1��   H�D$H�D$|L��H�D$p    H�D$    H�D$H�$    ������t%�D$|A�}   �u��A��H�|$`1��[����o���H�D$p�xt@ A�~   H�p1������H�|$p�������H�D$h�0�@ A�   H�p1�����H�|$h�����롋L$LH�D$@H�D$p    �i�L�`��~H�P�:-u�z-u�z uL�`�i�H�=�}  ��o@ ��o@ H�������D�k~  I��H�D$h    D�D$8����D�D$8I��H�D$pH�T$h�   �u@ H�D$L��1�H�D$     H�D$@   H�D$    H�$    ���������   ��u1�5�}  �Ў@ 1������ ���������H�D$h��t@ H�p����H�D$`E1�E1�1�1��   H�D$H�D$|L��H�D$`    H�D$    H�D$H�$    �Y�����t%�D$|A�}   �u��A��H�|$p1���������H�D$`�xt@ A�~   H�p1�����H�|$`�;�����H�D$h��t@ H�p1�A�   �����H�|$h����롋L$LH�D$@H�D$p    �i�L�`��~H�P�:-u�z-u�z uL�`�i�H�=|  ��o@ ��o@ H���@���H�D$h    I�������H�T$pE1�E1�H����t@ 1�H�T$H�T$hL��H�D$    H�$@   �g�����tQ��������P�@ 1������    � ���.�����H�D$p�xt@ A�~   H�p1�����H�|$p�'�������H�D$h�0�@ H�p��������1�I��^H��H���PTI��`f@ H���e@ H�� @ �C������H��H��s  H��t��H��Ð��������UH��SH���=�z   uK� �` H��z  H����` H��H��H9�s$fD  H��H��z  ����` H��z  H9�r���z  H��[]�fff.�     H�=@q   UH��t�    H��t]��` ��]Ð���������������f@ 1������@ SH��H�?�D���H�{(�;���H�{0�2���H�{8�i���H�{@�`���H���   �T���H���   ����H���   �����H���   �����H���   �����H��   