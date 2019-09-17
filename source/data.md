# いろんなデータ

## 階乗

$N$       $N!$
-------   ------
0         1
1         1
2         2
3         6
4         24
5         120
6         720
7         5040
8         40320
9         362880
10        3628800

## 数単位変換

N          日本語   英語
------     ------  ------
$10^{0}$    一      one
$10^{1}$    十      ten
$10^{2}$    百      hundred
$10^{3}$    千      thousand
$10^{4}$    万      ten thousand
$10^{5}$    十万    hundred thousand
$10^{6}$    百万    million
$10^{7}$    千万    ten million
$10^{8}$    億      hundred million
$10^{9}$    十億    billion
$10^{10}$   百億    ten billion
$10^{11}$   千億    hundred billion

## bit

N      $2^N$                            備考
------ ----------------------------     --------------------------------------------
0      1
1      2                                boolの大きさ
2      4
3      8
4      16
7      128                              charの最大値+1
8      256                              unsigned charの最大値+1
16     65,534
24     16,777,216
31     2,147,483,648                    intの最大値+1(about 2*10^9)
32     4,294,967,296                    unsigned intの最大値+1
52     4,503,599,627,370,496            doubleのprecision
63     9,223,372,036,854,775,808        long longの最大値+1
64     18,446,744,073,709,551,616       unsigned long longの最大値+1(about 10^19)

## 最低限の設定ファイル

### vim用
最低限のもの。ホームにおく。

~~~~~~
set fileencoding=utf-8
set nocompatible
set t_Co=256
set ambiwidth=double

syntax on
filetype plugin indent on

set nobackup
set noswapfile

set completeopt=menuone
set wildmode=list:longest

set smartindent
set autoindent
set tabstop=4
set softtabstop=4
set shiftwidth=4

set smarttab
set expandtab

set incsearch ignorecase hlsearch
set showmatch
set wildmenu

set listchars=tab:>-,trail:-
set list

set backspace=indent,eol,start
~~~~~~

<!-- 
## アスキーコード


Char   Dec    Oct    Hex
------ ------ ------ ------
(nul)    0     0000   0x00
(soh)    1     0001   0x01
(stx)    2     0002   0x02
(etx)    3     0003   0x03
(eot)    4     0004   0x04
(enq)    5     0005   0x05
(ack)    6     0006   0x06
(bel)    7     0007   0x07
(bs)     8     0010   0x08
(ht)     9     0011   0x09
(nl)    10     0012   0x0a
(vt)    11     0013   0x0b
(np)    12     0014   0x0c
(cr)    13     0015   0x0d
(so)    14     0016   0x0e
(si)    15     0017   0x0f
(dle)   16     0020   0x10
(dc1)   17     0021   0x11
(dc2)   18     0022   0x12
(dc3)   19     0023   0x13
(dc4)   20     0024   0x14
(nak)   21     0025   0x15
(syn)   22     0026   0x16
(etb)   23     0027   0x17
(can)   24     0030   0x18
(em)    25     0031   0x19
(sub)   26     0032   0x1a
(esc)   27     0033   0x1b
(fs)    28     0034   0x1c
(gs)    29     0035   0x1d
(rs)    30     0036   0x1e
(us)    31     0037   0x1f
(sp)    32     0040   0x20
!       33     0041   0x21
"       34     0042   0x22
#       35     0043   0x23
$       36     0044   0x24
%       37     0045   0x25
&       38     0046   0x26
'       39     0047   0x27
(       40     0050   0x28
)       41     0051   0x29
*       42     0052   0x2a
+       43     0053   0x2b
,       44     0054   0x2c
-       45     0055   0x2d
.       46     0056   0x2e
/       47     0057   0x2f
0       48     0060   0x30
1       49     0061   0x31
2       50     0062   0x32
3       51     0063   0x33
4       52     0064   0x34
5       53     0065   0x35
6       54     0066   0x36
7       55     0067   0x37
8       56     0070   0x38
9       57     0071   0x39
:       58     0072   0x3a
;       59     0073   0x3b
<       60     0074   0x3c
=       61     0075   0x3d
>       62     0076   0x3e
?       63     0077   0x3f
@       64     0100   0x40
A       65     0101   0x41
B       66     0102   0x42
C       67     0103   0x43
D       68     0104   0x44
E       69     0105   0x45
F       70     0106   0x46
G       71     0107   0x47
H       72     0110   0x48
I       73     0111   0x49
J       74     0112   0x4a
K       75     0113   0x4b
L       76     0114   0x4c
M       77     0115   0x4d
N       78     0116   0x4e
O       79     0117   0x4f
P       80     0120   0x50
Q       81     0121   0x51
R       82     0122   0x52
S       83     0123   0x53
T       84     0124   0x54
U       85     0125   0x55
V       86     0126   0x56
W       87     0127   0x57
X       88     0130   0x58
Y       89     0131   0x59
Z       90     0132   0x5a
[       91     0133   0x5b
\       92     0134   0x5c
]       93     0135   0x5d
^       94     0136   0x5e
_       95     0137   0x5f
`       96     0140   0x60
a       97     0141   0x61
b       98     0142   0x62
c       99     0143   0x63
d      100     0144   0x64
e      101     0145   0x65
f      102     0146   0x66
g      103     0147   0x67
h      104     0150   0x68
i      105     0151   0x69
j      106     0152   0x6a
k      107     0153   0x6b
l      108     0154   0x6c
m      109     0155   0x6d
n      110     0156   0x6e
o      111     0157   0x6f
p      112     0160   0x70
q      113     0161   0x71
r      114     0162   0x72
s      115     0163   0x73
t      116     0164   0x74
u      117     0165   0x75
v      118     0166   0x76
w      119     0167   0x77
x      120     0170   0x78
y      121     0171   0x79
z      122     0172   0x7a
{      123     0173   0x7b
|      124     0174   0x7c
}      125     0175   0x7d
~      126     0176   0x7e
(del)  127     0177   0x7f
-->
