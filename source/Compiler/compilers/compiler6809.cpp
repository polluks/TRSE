#include "compiler6809.h"

void Compiler6809::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_codeGen = QSharedPointer<CodeGen6809>(new CodeGen6809());
    m_assembler = QSharedPointer<Asm6809>(new Asm6809());
    bool useOrgasm = m_ini->getString("assembler_6809").toLower()=="orgasm";
    m_assembler->m_isOrgasm = useOrgasm;
    m_assembler->byte="fcb";
    m_assembler->word="fdb";
    if (useOrgasm) {
        m_assembler->byte="db";
        m_assembler->word="dw";


    }

    m_assembler->m_zbyte = 0x10;

    Init6809Assembler();
    LabelStack::m_labelCount = 0;
}

void Compiler6809::Connect()
{
    bool useMorgasm = (m_ini->getString("assembler_6809")=="OrgAsm");
    //if (!useMorgasm)
     m_assembler->IncludeFile(":resources/code/6809/mul16.asm");
    m_assembler->Connect();
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VECTREX) {

//        m_assembler->m_source << "\torg $C800";
        m_assembler->m_source <<m_assembler->m_wram->m_source;
    }

    if (m_ini->getString("assembler_6809")=="lwasm")
        m_assembler->m_source<<"	END START";

    CleanupBlockLinenumbers();
/*    for (QString&s : m_assembler->m_source) {
        s = s.replace("sta(","sta (");
        s = s.replace("lda(","lda (");
    }
    */
}

void Compiler6809::CleanupCycleLinenumbers(QString currentFile, QHash<int, int> &ocycles, QHash<int, int> &retcycles, bool isCycles)
{
}

void Compiler6809::Init6809Assembler()
{
    if (m_projectIni->getdouble("override_target_settings")==1) {
          Syntax::s.m_currentSystem->m_startAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_basic"));
          Syntax::s.m_ignoreSys = m_projectIni->getdouble("override_target_settings_sys")==1;
          Syntax::s.m_currentSystem->m_programStartAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_org"));
          Syntax::s.m_currentSystem->m_stripPrg = m_projectIni->getdouble("override_target_settings_prg")==1;
          if (Syntax::s.m_ignoreSys)
              Syntax::s.m_currentSystem->m_startAddress = Syntax::s.m_currentSystem->m_programStartAddress;
      } else {
          Syntax::s.m_currentSystem->DefaultValues();
          Syntax::s.m_ignoreSys = Syntax::s.m_currentSystem->m_ignoreSys;
   //       Syntax::s.m_stripPrg = false;

      }
    // Special case: Thomson CART
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::THOMSON) {
        if (m_projectIni->getString("thomson_media")=="CART") {
          Syntax::s.m_currentSystem->m_startAddress = 0xB000;
          Syntax::s.m_currentSystem->m_programStartAddress = 0xB000;
        }

    }
    m_assembler->m_startInsertAssembler<<" org "+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress);
    m_assembler->m_startInsertAssembler << m_parser.m_initAssembler;
    m_assembler->m_startInsertAssembler << "START:";

    m_assembler->m_defines = m_parser.m_preprocessorDefines;


    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VECTREX) {
        m_assembler->IncludeFile(":resources/code/6809/vectrex_header.asm", true, true);
        bool useMorgasm = (m_ini->getString("assembler_6809")=="OrgAsm");
        if (useMorgasm)
            for (QString&s : m_assembler->m_startInsertAssembler) {
                s = s.replace("fcc", ".dc");
                s = s.replace("fcb", ".byte");
            }

    }


//    m_assembler->InitZeroPointers(m_projectIni->getStringList("zeropages"),m_projectIni->getStringList("temp_zeropages"),m_projectIni->getStringList("var_zeropages"));
/*    qDebug() << m_projectIni->contains("ignore_initial_jump");
    for (CItem i : m_projectIni->items)
        qDebug() << i.name;
*/
    m_assembler->m_ignoreInitialJump = m_projectIni->getdouble("ignore_initial_jump")==1.0;


    if (Syntax::s.m_currentSystem->isCommodoreSystem() && !Syntax::s.m_ignoreSys)
        Syntax::s.m_currentSystem->m_startAddress = Syntax::s.m_currentSystem->getDefaultBasicAddress();

    Syntax::s.m_ignoreSys=true;
    Syntax::s.m_currentSystem->m_ignoreSys = true;;
}


int Compiler6809::FindEndSymbol(Orgasm &orgasm)
{
    //    QStringList output = QString(out).split("\n");
    for (QString s : orgasm.m_symbols.keys()) {
        if (s.toLower().contains("endsymbol")) {
            return orgasm.m_symbols[s];
        }
    }
    return 0;
}

QVector<int> Compiler6809::FindBlockEndSymbols(Orgasm &orgasm)
{
    QVector<int> m_blockEndSymbols;

    m_blockEndSymbols.clear();
    for (QString s : orgasm.m_symbols.keys()) {
        if (s.toLower().contains("endblock")) {
            QString spl = s;
            spl = spl.toLower().simplified().split("block")[1];
            //            bool ok;
            int i= orgasm.m_symbols[s];//.toInt(&ok, 16);
            //qDebug() << "FOUND endblock : " << s << Util::numToHex(i);
            m_blockEndSymbols.append(i);
        }
    }
    return m_blockEndSymbols;
}

void Compiler6809::ConnectBlockSymbols(QVector<int> &blockEndSymbols)
{
    for (int sym : blockEndSymbols) {
        int winner = 0xFFFF;
        QSharedPointer<MemoryBlock> winnerBlock=nullptr;

        for (QSharedPointer<MemoryBlock> mb: m_assembler->blocks) {
            //            if (mb->m_type==MemoryBlock::CODE &&  sym>mb->m_start)
            if (sym>mb->m_start)
                if (sym-mb->m_start<winner) {
                    winner = sym-mb->m_start;
                    winnerBlock  =mb;
                }
        }
        if (winnerBlock!=nullptr) {
            winnerBlock->m_end = sym;
            //     qDebug() << Util::numToHex(sym) << " " << winnerBlock->Type();
        }
    }



}
