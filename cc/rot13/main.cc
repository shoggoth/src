//
//  main.cc
//  Sky tech test
//
//  Created by Richard Henry on 18/01/2021.
//  Copyright 2021 Dogstar Industries Ltd. All rights reserved.
//

#include <iostream>
#include <string>

static std::string encode(std::string s);

int main() {

    std::string encodedString = "Uv gurer,\n\nSvefg bs nyy: gunax lbh sbe fraqvat zr gur grpuavpny grfg bire naq sbe pbafvqrevat zr sbe guvf cbfvgvba.\n\n"
    "V'ir unq n ybbx ng gur grfg naq jbexrq bhg gung vg'q vaibyir vqragvslvat n p++ zbpxvat senzrjbex naq jevgvat n ahzore bs havg grfgf sbe n snveyl "
    "zvavzny p++ onpxraq juvpu jbhyq or whfg n fjvgpu fgngrzrag. Svthevat gung guvf qbrfa'g ernyyl cynl gb zl fgeratguf, V'yy or ubarfg jvgu lbh naq "
    "fnl gung vs guvf vf vaqvpngvir bs jung'f n znwbevgl bs gur jbex lbhe pbagenpgbe jvyy or tvira gura V'z abg ernyyl gur ynq sbe lbh naq V jba'g jnfgr "
    "nal zber bs lbhe gvzr.\n\nVs, ubjrire, lbh'er ybbxvat sbe na ratvarre jub'f vairagvir naq jubfr fgeratguf ner qrfvtavat naq jevgvat eryvnoyr naq "
    "rssvpvrag fbsgjner jvgu na vzntvangvir nccebnpu gura V ubcr V pna bssre lbh guvf sentzrag bs zbqrea p++ pbqr nf rivqrapr bs gung. V jvyy nyfb bssre "
    "n zber genqvgvbany p++ sentzrag gung V jebgr dhvgr n ahzore bs lrnef ntb gb fbyir fhqbxh chmmyrf: lbh pna svaq gur fbhepr naq n jro-onfrq qrzb ng "
    "gur sbyybjvat nqqerff:\n\nuggcf://jjj.qbtfgne.zbov/pbqr/fhqbxh\n\nGubhtu byq, vg hfrf zber genqvgvbany p++, vf pbapvfr naq cresbeznag naq vf fbzrguvat "
    "V qrfvtarq sebz gur tebhaq hc.\n\nFrpbaqyl, naq hasbeghangryl, V nz trggvat n cerggl ybj-raretl ivor sebz gur erpehvgvat ntrag ba guvf wbo. Vg'f znqr zr "
    "srry yvxr ur vf whfg qbvat gur zvavzhz naq unfa'g ernyyl tbg nal pbasvqrapr gung ur'yy cynpr nalbar. Guvf unf abg orra n znffvir zbgvingbe sbe zr nf V "
    "unir qbar n ybg bs jbex sbe serr guvf ynfg lrne. Qba'g zragvba jung V'ir fnvq boivbhfyl vg'f jul V'ir boshfpngrq guvf grkg, gevivny gubhtu gur boshfpngvba "
    "vf, vg'f rabhtu gb xrrc vg sebz gur rlrf bs aba-cebtf.\n\nV ubcr lbh haqrefgnaq zl vagragvbaf naq nccerpvngr zl qverpgarff va gur fcvevg vg jnf vagraqrq. "
    "V'yy pbzcyrgryl haqrefgnaq vs lbh'er ybbxvat sbe fbzrbar jub'f zber vagb gur Ntvyr zvaqfrg sbe guvf wbo ohg vs lbh'q pbafvqre n fgebat Ratvarre gura V "
    "ubcr gur gjb pbqr sentzragf jbhyq tvir hf fbzrguvat gb gnyx nobhg va n shegure qvfphffvba.\n\nOrfg ertneqf,\n\nEvpuneq Urael";

    std::cout << encode(encodedString) << std::endl;

    return 0;
}

std::string encode(std::string s) {

    std::transform(begin(s), end(s), begin(s), [] (char c) -> char {

            if (not std::isalpha(c)) return c;

            char const base = std::isupper(c) ? 'A' : 'a';

            return (c - base + 13) % 26 + base;
        });

    return s;
}
