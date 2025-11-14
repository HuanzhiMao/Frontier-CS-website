#include <iostream>

int main() {
    std::cout << R"AB_PROGRAM(
S=SM
Ma=aM
Mb=bM
Mc=cM
M=K
Ka=aKa
Kb=bKb
Kc=cKc
K=

S=C
aCa=C
bCb=C
cCc=C
CM=(return)1
C=F
Fa=F
Fb=F
Fc=F
FM=G
aG=Ga
bG=Gb
cG=Gc
SG=GS
Ga=H
Gb=H
Gc=H
H=
S=C
CS=K
Ka=aKa
Kb=bKb
Kc=cKc
K=M
SM=(return)0
=(return)0
)AB_PROGRAM";
    return 0;
}