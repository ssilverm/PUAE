/*
 * PUAE - The *nix Amiga Emulator
 *
 * QT GUI for PUAE
 *
 * Copyright 2010 Mustafa 'GnoStiC' TUFAN
 * (GUI layout cloned from WinUAE/Toni Wilen)
 *
 */

#include "puae_mainwindow.h"
#include "ui_puae_mainwindow.h"

#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

extern "C" {
#include "include/options.h"
extern int candirect;
extern bool canbang;
struct uae_prefs workprefs;
}

// defs from custom.h
/* These are the masks that are ORed together in the chipset_mask option.
 * If CSMASK_AGA is set, the ECS bits are guaranteed to be set as well.  */
#define CSMASK_ECS_AGNUS 1
#define CSMASK_ECS_DENISE 2
#define CSMASK_AGA 4
#define CSMASK_MASK (CSMASK_ECS_AGNUS | CSMASK_ECS_DENISE | CSMASK_AGA)

// Paths Tab
QString PATHS_ROM, PATHS_CONFIG, PATHS_SCREENSHOT, PATHS_SAVESTATE, PATHS_AVIOUTPUT, PATHS_SAVEIMAGE, PATHS_RIP;

// mem
static const char *memsize_names[] = {
        /* 0 */ "none",
        /* 1 */ "256 K",
        /* 2 */ "512 K",
        /* 3 */ "1 MB",
        /* 4 */ "2 MB",
        /* 5 */ "4 MB",
        /* 6 */ "8 MB",
        /* 7 */ "16 MB",
        /* 8 */ "32 MB",
        /* 9 */ "64 MB",
        /* 10*/ "128 MB",
        /* 11*/ "256 MB",
        /* 12*/ "512 MB",
        /* 13*/ "1 GB",
        /* 14*/ "1.5MB",
        /* 15*/ "1.8MB",
        /* 16*/ "2 GB",
        /* 17*/ "384 MB",
        /* 18*/ "768 MB",
        /* 19*/ "1.5 GB",
        /* 20*/ "2.5 GB",
        /* 21*/ "3 GB"
};

static unsigned long memsizes[] = {
        /* 0 */ 0,
        /* 1 */ 0x00040000, /* 256K */
        /* 2 */ 0x00080000, /* 512K */
        /* 3 */ 0x00100000, /* 1M */
        /* 4 */ 0x00200000, /* 2M */
        /* 5 */ 0x00400000, /* 4M */
        /* 6 */ 0x00800000, /* 8M */
        /* 7 */ 0x01000000, /* 16M */
        /* 8 */ 0x02000000, /* 32M */
        /* 9 */ 0x04000000, /* 64M */
        /* 10*/ 0x08000000, //128M
        /* 11*/ 0x10000000, //256M
        /* 12*/ 0x20000000, //512M
        /* 13*/ 0x40000000, //1GB
        /* 14*/ 0x00180000, //1.5MB
        /* 15*/ 0x001C0000, //1.8MB
        /* 16*/ 0x80000000, //2GB
        /* 17*/ 0x18000000, //384M
        /* 18*/ 0x30000000, //768M
        /* 19*/ 0x60000000, //1.5GB
        /* 20*/ 0xA8000000, //2.5GB
        /* 21*/ 0xC0000000, //3GB
};

static int msi_chip[] = { 1, 2, 3, 14, 4, 5, 6 };
static int msi_bogo[] = { 0, 2, 3, 14, 15 };
static int msi_fast[] = { 0, 3, 4, 5, 6 };
static int msi_z3fast[] = { 0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 17, 12, 18, 13, 19, 16, 20, 21 };
static int msi_z3chip[] = { 0, 7, 8, 9, 10, 11, 12, 13 };
static int msi_gfx[] = { 0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

puae_MainWindow::puae_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::puae_MainWindow)
{
    ui->setupUi(this);

    QString myPath;
    myPath = QDir::currentPath();

    // Paths Tab
    PATHS_ROM = myPath;
    PATHS_CONFIG = myPath;
    PATHS_SCREENSHOT = myPath;
    PATHS_SAVESTATE = myPath;
    PATHS_AVIOUTPUT = myPath;
    PATHS_SAVEIMAGE = myPath;
    PATHS_RIP = myPath;
}

puae_MainWindow::~puae_MainWindow()
{
    delete ui;
}

//
//
//
QString puae_MainWindow::GetPath(QWidget *who, QString what, QString where)
{
    QString path = QFileDialog::getExistingDirectory(who, what, where, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if ( path.isNull() ) {
        path = "./";
    }

    return path;
}

QString puae_MainWindow::GetFile(QWidget *who, QString what, QString extensions)
{
    QString file = QFileDialog::getOpenFileName(who, what, PATHS_ROM, extensions);

//    if ( !file.isNull() ) {
    return file;
}

//*************************************************
// PATHS TAB
//*************************************************

/* Choose System ROMS Path */
void puae_MainWindow::on_IDC_PATHS_ROMS_clicked()
{
    PATHS_ROM = GetPath(this, "Select System ROMS Path", PATHS_ROM);
    ui->IDC_PATHS_ROM->setText (PATHS_ROM);
}

/* Choose Configuration Files Path */
void puae_MainWindow::on_IDC_PATHS_CONFIGS_clicked()
{
    PATHS_CONFIG = GetPath(this, "Select Config Path", PATHS_CONFIG);
    ui->IDC_PATHS_CONFIG->setText (PATHS_CONFIG);
}

/* Choose Screenshot Path */
void puae_MainWindow::on_IDC_PATHS_SCREENSHOTS_clicked()
{
    PATHS_SCREENSHOT = GetPath(this, "Select Screenshot Path", PATHS_SCREENSHOT);
    ui->IDC_PATHS_SCREENSHOT->setText (PATHS_SCREENSHOT);
}

/* Choose Savestate Path */
void puae_MainWindow::on_IDC_PATHS_SAVESTATES_clicked()
{
    PATHS_SAVESTATE = GetPath(this, "Select Savestate Path", PATHS_SAVESTATE);
    ui->IDC_PATHS_SAVESTATE->setText (PATHS_SAVESTATE);
}

/* Choose AVI Output Path */
void puae_MainWindow::on_IDC_PATHS_AVIOUTPUTS_clicked()
{
    PATHS_AVIOUTPUT = GetPath(this, "Select AVI Output Path", PATHS_AVIOUTPUT);
    ui->IDC_PATHS_AVIOUTPUT->setText (PATHS_AVIOUTPUT);
}

/* Choose Save Images Path */
void puae_MainWindow::on_IDC_PATHS_SAVEIMAGES_clicked()
{
    PATHS_SAVEIMAGE = GetPath(this, "Select Save Image Path", PATHS_SAVEIMAGE);
    ui->IDC_PATHS_SAVEIMAGE->setText (PATHS_SAVEIMAGE);
}

/* Choose RIP Path */
void puae_MainWindow::on_IDC_PATHS_RIPS_clicked()
{
    PATHS_RIP = GetPath(this, "Select RIP Path", PATHS_RIP);
    ui->IDC_PATHS_RIP->setText (PATHS_RIP);
}

//*************************************************
// ROMS TAB
//*************************************************

/* Choose Main ROM File */
void puae_MainWindow::on_IDC_KICKCHOOSER_clicked()
{
    QString fileName = GetFile (this, "Select Main ROM", "ROM Files (*.rom)");
    //ui->IDC_ROMFILE->setText(fileName);

}

/* Choose Extended ROM File */
void puae_MainWindow::on_IDC_ROMCHOOSER2_clicked()
{
    QString fileName = GetFile (this, "Select Extended ROM", "ROM Files (*.rom)");

}

/* Choose Cartridge ROM File */
void puae_MainWindow::on_IDC_CARTCHOOSER_clicked()
{
    QString fileName = GetFile (this, "Select Cartridge ROM File", "ROM Files (*.rom)");

}

/* Choose Flash RAM File */
void puae_MainWindow::on_IDC_FLASHCHOOSER_clicked()
{
    QString fileName = GetFile (this, "Select Flash RAM File", "RAM Files (*.ram)");

}

/* Map ROM Emulation */
void puae_MainWindow::on_IDC_MAPROM_toggled(bool ischecked)
{
    workprefs.maprom = ischecked ? 0x0f000000 : 0;
}

/* Shapeshifter support */
void puae_MainWindow::on_IDC_KICKSHIFTER_toggled(bool ischecked)
{
    workprefs.kickshifter = ischecked;
}

//*************************************************
// HW-CPU
//*************************************************

/* CPU 68000 */
void puae_MainWindow::on_IDC_CPU0_clicked()
{
    workprefs.cpu_model = 0;
}

/* CPU 68010 */
void puae_MainWindow::on_IDC_CPU1_clicked()
{
    workprefs.cpu_model = 1;
}

/* CPU 68020 */
void puae_MainWindow::on_IDC_CPU2_clicked()
{
    workprefs.cpu_model = 2;
}

/* CPU 68030 */
void puae_MainWindow::on_IDC_CPU3_clicked()
{
    workprefs.cpu_model = 3;
}

/* CPU 68040 */
void puae_MainWindow::on_IDC_CPU4_clicked()
{
    workprefs.cpu_model = 4;
}

/* CPU 68060 */
void puae_MainWindow::on_IDC_CPU5_clicked()
{
    workprefs.cpu_model = 5;
}

/* 24-bit Addressing */
void puae_MainWindow::on_IDC_COMPATIBLE24_toggled(bool ischecked)
{
    workprefs.address_space_24 = ischecked;
}

/* More Compatible */
void puae_MainWindow::on_IDC_COMPATIBLE_toggled(bool ischecked)
{
    workprefs.cpu_compatible = ischecked;
}

/* JIT Enable */
void puae_MainWindow::on_IDC_JITENABLE_toggled(bool ischecked)
{
//    cachesize_prev = workprefs.cachesize;
//    trust_prev = workprefs.comptrustbyte;

    workprefs.cachesize = 0;
}

/*68040 MMU Enable */
void puae_MainWindow::on_IDC_MMUENABLE_toggled(bool ischecked)
{
    //workprefs
}

/* FPU None */
void puae_MainWindow::on_IDC_FPU0_clicked()
{
    workprefs.mmu_model = 0;
}

/* FPU 68881 */
void puae_MainWindow::on_IDC_FPU1_clicked()
{
    workprefs.mmu_model = 1;
}

/* FPU 68882 */
void puae_MainWindow::on_IDC_FPU2_clicked()
{
    workprefs.mmu_model = 2;
}

/* FPU Internal */
void puae_MainWindow::on_IDC_FPU3_clicked()
{
    workprefs.mmu_model = 3;
}

/* FPU More Compatible */
void puae_MainWindow::on_IDC_COMPATIBLE_FPU_toggled(bool ischecked)
{
    //workprefs.fpu_strict = ischecked;
}

/* CPU Fastest Possible */
void puae_MainWindow::on_IDC_CS_HOST_clicked()
{
    workprefs.m68k_speed = -1;
}

/* CPU Cycle-Exact */
void puae_MainWindow::on_IDC_CS_68000_clicked()
{
    workprefs.m68k_speed = 0;
}

/* CPU Adjustable */
void puae_MainWindow::on_IDC_CS_ADJUSTABLE_clicked()
{
    workprefs.m68k_speed = 1;
}

/* CPU CPU<-->Chipset */
void puae_MainWindow::on_IDC_SPEED_valueChanged(int value)
{

}

/* CPU CPU Idle*/
void puae_MainWindow::on_IDC_CPUIDLE_sliderMoved(int position)
{

}

/* CPU CE */
void puae_MainWindow::on_IDC_CPU_FREQUENCY_currentIndexChanged(int index)
{

}

//*************************************************
// HW-Chipset
//*************************************************

/* OCS */
void puae_MainWindow::on_IDC_OCS_clicked()
{
    workprefs.chipset_mask = 0;
}

/* ECS AGNUS */
void puae_MainWindow::on_IDC_ECS_AGNUS_clicked()
{
    workprefs.chipset_mask = 1;
}

/* ECS DENISE */
void puae_MainWindow::on_IDC_ECS_DENISE_clicked()
{
    workprefs.chipset_mask = 2;
}

/* ECS FULL = AGNUS + DENISE */
void puae_MainWindow::on_IDC_ECS_clicked()
{
    workprefs.chipset_mask = 3;
}

/* AGA */
void puae_MainWindow::on_IDC_AGA_clicked()
{
    workprefs.chipset_mask = 4;
}

/* NTSC */
void puae_MainWindow::on_IDC_NTSC_toggled(bool ischecked)
{
    workprefs.ntscmode = ischecked;
}

/* Chipset Extra */
void puae_MainWindow::on_IDC_CS_EXT_currentIndexChanged(int index)
{

}

/* Blitter Immediate */
void puae_MainWindow::on_IDC_BLITIMM_toggled(bool ischecked)
{
    workprefs.immediate_blits = ischecked;
}

/* Cycle Exact */
void puae_MainWindow::on_IDC_CYCLEEXACT_toggled(bool ischecked)
{
    workprefs.cpu_cycle_exact = ischecked;
}

/* Genlock Enabled */
void puae_MainWindow::on_IDC_GENLOCK_toggled(bool ischecked)
{
    workprefs.genlock = ischecked;
}

/* Sprite Collision None */
void puae_MainWindow::on_IDC_COLLISION0_clicked()
{
    workprefs.collision_level = 0;
}

/* Sprites Only */
void puae_MainWindow::on_IDC_COLLISION1_clicked()
{
    workprefs.collision_level = 1;
}

/* Sprites and Sprites vs Playfield */
void puae_MainWindow::on_IDC_COLLISION2_clicked()
{
    workprefs.collision_level = 2;
}

/* Collision Full */
void puae_MainWindow::on_IDC_COLLISION3_clicked()
{
    workprefs.collision_level = 3;
}

/* Sound Disabled */
void puae_MainWindow::on_IDC_CS_SOUND0_clicked()
{
    workprefs.produce_sound = 0;
}

/* Sound Emulated */
void puae_MainWindow::on_IDC_CS_SOUND1_clicked()
{
    workprefs.produce_sound = 1;
}

/* Sound Emulated %100 */
void puae_MainWindow::on_IDC_CS_SOUND2_clicked()
{
    workprefs.produce_sound = 2;
}

//*************************************************
// Advanced Chipset
//*************************************************

/* Compatible Settings */
void puae_MainWindow::on_IDC_CS_COMPATIBLE_toggled(bool ischecked)
{
    workprefs.cs_compatible = ischecked;
}

/* Battery Backed Real Time Clock None */
void puae_MainWindow::on_IDC_CS_RTC1_clicked()
{
    workprefs.cs_rtc = 0;
}

/* Battery Backed Real Time Clock None */
void puae_MainWindow::on_IDC_CS_RTC2_clicked()
{
    workprefs.cs_rtc = 1;
}

/* Battery Backed Real Time Clock None */
void puae_MainWindow::on_IDC_CS_RTC3_clicked()
{
    workprefs.cs_rtc = 2;
}

/* CIA-A TOD Clock Source */
void puae_MainWindow::on_IDC_CS_CIAA_TOD1_clicked()
{
    workprefs.cs_ciaatod = 0;
}

/* CIA-A TOD Clock Source */
void puae_MainWindow::on_IDC_CS_CIAA_TOD2_clicked()
{
    workprefs.cs_ciaatod = 1;
}

/* CIA-A TOD Clock Source */
void puae_MainWindow::on_IDC_CS_CIAA_TOD3_clicked()
{
    workprefs.cs_ciaatod = 2;
}

/* CIA ROM Overlay */
void puae_MainWindow::on_IDC_CS_CIAOVERLAY_toggled(bool ischecked)
{
    workprefs.cs_ciaoverlay = ischecked;
}

/* CD32 CD */
void puae_MainWindow::on_IDC_CS_CD32CD_toggled(bool ischecked)
{
    workprefs.cs_cd32cd = ischecked;
}

void puae_MainWindow::on_IDC_CS_CD32C2P_toggled(bool ischecked)
{
    workprefs.cs_cd32c2p = ischecked;
}

void puae_MainWindow::on_IDC_CS_CD32NVRAM_toggled(bool ischecked)
{
    workprefs.cs_cd32nvram = ischecked;
}

void puae_MainWindow::on_IDC_CS_CDTVCD_toggled(bool ischecked)
{
    workprefs.cs_cdtvcd = ischecked;
}

void puae_MainWindow::on_IDC_CS_CDTVRAM_toggled(bool ischecked)
{
    workprefs.cs_cdtvram = ischecked;
}

void puae_MainWindow::on_IDC_CS_IDE1_toggled(bool ischecked)
{
    workprefs.cs_ide = (ui->IDC_CS_IDE1->checkState()) ? 1 : ((ui->IDC_CS_IDE2->checkState()) ? 2 : 0);
}

void puae_MainWindow::on_IDC_CS_IDE2_toggled(bool ischecked)
{
    workprefs.cs_ide = (ui->IDC_CS_IDE1->checkState()) ? 1 : ((ui->IDC_CS_IDE2->checkState()) ? 2 : 0);
}

/* ROM Mirror E0 */
void puae_MainWindow::on_IDC_CS_KSMIRROR_E0_toggled(bool ischecked)
{
    workprefs.cs_ksmirror_e0 = ischecked;
}

/* ROM Mirror A8 */
void puae_MainWindow::on_IDC_CS_KSMIRROR_A8_toggled(bool ischecked)
{
    workprefs.cs_ksmirror_a8 = ischecked;
}

/* C00000 is Fast RAM */
void puae_MainWindow::on_IDC_CS_SLOWISFAST_toggled(bool ischecked)
{
    workprefs.cs_slowmemisfast = ischecked;
}

/* A1000 Boot ROM/RAM */
void puae_MainWindow::on_IDC_CS_A1000RAM_toggled(bool ischecked)
{
    workprefs.cs_a1000ram = ischecked;
}

/* DF0: ID Hardware */
void puae_MainWindow::on_IDC_CS_DF0IDHW_toggled(bool ischecked)
{
    workprefs.cs_df0idhw = ischecked;
}

/* CDTV SRAM Expansion */
void puae_MainWindow::on_IDC_CS_CDTVRAMEXP_toggled(bool ischecked)
{
    workprefs.cs_cdtvram = ischecked;
}

/* PCMCIA */
void puae_MainWindow::on_IDC_CS_PCMCIA_toggled(bool ischecked)
{
    workprefs.cs_pcmcia = ischecked;
}

/* KB Reset Warning */
void puae_MainWindow::on_IDC_CS_RESETWARNING_toggled(bool ischecked)
{
    workprefs.cs_resetwarning = ischecked;
}

/* No-EHB DENISE */
void puae_MainWindow::on_IDC_CS_NOEHB_toggled(bool ischecked)
{
    workprefs.cs_denisenoehb = ischecked;
}

/* A1000 Agnus */
void puae_MainWindow::on_IDC_CS_DIPAGNUS_toggled(bool ischecked)
{
    workprefs.cs_dipagnus = ischecked;
}

/* A590/A2091 SCSI */
void puae_MainWindow::on_IDC_CS_A2091_toggled(bool ischecked)
{
    workprefs.cs_a2091 = ischecked;
}

/* A3000 SCSI */
void puae_MainWindow::on_IDC_CS_DMAC_toggled(bool ischecked)
{
    workprefs.cs_mbdmac = (ui->IDC_CS_DMAC->checkState()) ? 1 : ((ui->IDC_CS_DMAC2->checkState()) ? 2 : 0);
}

/* A4000T SCSI */
void puae_MainWindow::on_IDC_CS_DMAC2_toggled(bool ischecked)
{
    workprefs.cs_mbdmac = (ui->IDC_CS_DMAC->checkState()) ? 1 : ((ui->IDC_CS_DMAC2->checkState()) ? 2 : 0);
}

/* A4901 SCSI */
void puae_MainWindow::on_IDC_CS_A4091_toggled(bool ischecked)
{
    workprefs.cs_a4091 = ischecked;
}

/* CDTV SCSI */
void puae_MainWindow::on_IDC_CS_CDTVSCSI_toggled(bool ischecked)
{
    workprefs.cs_cdtvscsi = ischecked;
}

/* Ramsey Revision */
void puae_MainWindow::on_IDC_CS_RAMSEY_toggled(bool ischecked)
{
    workprefs.cs_ramseyrev = ischecked ? 0x0f : -1;
    //ui->IDC_CS_RAMSEYREV->setText(workprefs.cs_ramseyrev);
}

/* Fat Gary Revision */
void puae_MainWindow::on_IDC_CS_FATGARY_toggled(bool ischecked)
{
    workprefs.cs_fatgaryrev = ischecked ? 0x00 : -1;
}

/* Agnus/Alice Revision */
void puae_MainWindow::on_IDC_CS_AGNUS_toggled(bool checked)
{

}

/* Denise/Lisa Revision */
void puae_MainWindow::on_IDC_CS_DENISE_toggled(bool checked)
{

}

//*************************************************
// RAM
//*************************************************

/* Chip RAM */
void puae_MainWindow::on_IDC_CHIPMEM_valueChanged(int value)
{
    workprefs.chipmem_size = memsizes[msi_chip[value]];
    ui->IDC_CHIPRAM->setText(memsize_names[msi_chip[value]]);
}

/* Fast RAM */
void puae_MainWindow::on_IDC_FASTMEM_valueChanged(int value)
{
    workprefs.fastmem_size = memsizes[msi_fast[value]];
    ui->IDC_FASTRAM->setText(memsize_names[msi_fast[value]]);
}

/* Slow RAM */
void puae_MainWindow::on_IDC_SLOWMEM_valueChanged(int value)
{
    workprefs.bogomem_size = memsizes[msi_bogo[value]];
    ui->IDC_SLOWRAM->setText(memsize_names[msi_bogo[value]]);
}

/* Z3 Fast RAM */
void puae_MainWindow::on_IDC_Z3FASTMEM_valueChanged(int value)
{
    workprefs.z3chipmem_size = memsizes[msi_z3fast[value]];
    ui->IDC_Z3FASTRAM->setText(memsize_names[msi_z3fast[value]]);
}

/* Motherboard Fast RAM */
void puae_MainWindow::on_IDC_MBMEM1_valueChanged(int value)
{
    workprefs.mbresmem_low_size = memsizes[msi_gfx[value]];
    ui->IDC_MBRAM1->setText(memsize_names[msi_gfx[value]]);
}

/* Processor Slot RAM */
void puae_MainWindow::on_IDC_MBMEM2_valueChanged(int value)
{
    workprefs.mbresmem_high_size = memsizes[msi_gfx[value]];
    ui->IDC_MBRAM2->setText(memsize_names[msi_gfx[value]]);
}

void puae_MainWindow::on_IDC_FLOPPYSPD_valueChanged(int value)
{
    workprefs.floppy_speed = value;
    if (workprefs.floppy_speed > 0) {
            workprefs.floppy_speed--;
            workprefs.floppy_speed = 1 << workprefs.floppy_speed;
            workprefs.floppy_speed *= 100;
    }

    out_floppyspeed();
}


//
// Santa's Little Helpers
//
int puae_MainWindow::getcpufreq (int m)
{
        int f;

        f = workprefs.ntscmode ? 28636360.0 : 28375160.0;
        return f * (m >> 8) / 8;
}

void puae_MainWindow::fix_values_memorydlg()
{
    if (workprefs.chipmem_size > 0x200000)
            workprefs.fastmem_size = 0;
}

void puae_MainWindow::updatez3 (unsigned int *size1p, unsigned int *size2p)
{
        int i;
        unsigned int s1, s2;

        // no 2GB Z3 size so we need 2x1G
        if (*size1p >= 0x80000000) {
                *size2p = *size1p - 0x40000000;
                *size1p = 0x40000000;
                return;
        }
        s1 = *size1p;
        *size1p = 0;
        *size2p = 0;
        s2 = 0;
        for (i = 32; i >= 0; i--) {
                if (s1 & (1 << i))
                        break;
        }
        if (i < 20)
                return;
        if (s1 == (1 << i)) {
                *size1p = s1;
                return;
        }
        s2 = s1 & ((1 << i) - 1);
        s1 = 1 << i;
        i--;
        while (i >= 0) {
                if (s2 & (1 << i)) {
                        s2 = 1 << i;
                        break;
                }
                i--;
        }
        if (i < 19)
                s2 = 0;
        *size1p = s1;
        *size2p = s2;
}

void puae_MainWindow::out_floppyspeed()
{
    char spe[30];

    if (workprefs.floppy_speed)
            sprintf (spe, "%d%%%s", workprefs.floppy_speed, workprefs.floppy_speed == 100 ? " (compatible)" : "");
    else
            strcpy (spe, "Turbo");
    ui->IDC_FLOPPYSPDTEXT->setText(spe);
}

void puae_MainWindow::values_to_memorydlg()
{
    unsigned int mem_size = 0;
    unsigned long v;

    switch (workprefs.chipmem_size) {
    case 0x00040000: mem_size = 0; break;
    case 0x00080000: mem_size = 1; break;
    case 0x00100000: mem_size = 2; break;
    case 0x00180000: mem_size = 3; break;
    case 0x00200000: mem_size = 4; break;
    case 0x00400000: mem_size = 5; break;
    case 0x00800000: mem_size = 6; break;
    }
    ui->IDC_CHIPMEM->setValue(mem_size);

    mem_size = 0;
    switch (workprefs.fastmem_size) {
    case 0x00000000: mem_size = 0; break;
    case 0x00100000: mem_size = 1; break;
    case 0x00200000: mem_size = 2; break;
    case 0x00400000: mem_size = 3; break;
    case 0x00800000: mem_size = 4; break;
    case 0x01000000: mem_size = 5; break;
    }
    ui->IDC_FASTMEM->setValue(mem_size);

    mem_size = 0;
    switch (workprefs.bogomem_size) {
    case 0x00000000: mem_size = 0; break;
    case 0x00080000: mem_size = 1; break;
    case 0x00100000: mem_size = 2; break;
    case 0x00180000: mem_size = 3; break;
    case 0x001C0000: mem_size = 4; break;
    }
    ui->IDC_SLOWMEM->setValue(mem_size);

    mem_size = 0;
    v = workprefs.z3fastmem_size + workprefs.z3fastmem2_size;
    if      (v < 0x00100000)
            mem_size = 0;
    else if (v < 0x00200000)
            mem_size = 1;
    else if (v < 0x00400000)
            mem_size = 2;
    else if (v < 0x00800000)
            mem_size = 3;
    else if (v < 0x01000000)
            mem_size = 4;
    else if (v < 0x02000000)
            mem_size = 5;
    else if (v < 0x04000000)
            mem_size = 6;
    else if (v < 0x08000000)
            mem_size = 7;
    else if (v < 0x10000000)
            mem_size = 8;
    else if (v < 0x18000000)
            mem_size = 9;
    else if (v < 0x20000000)
            mem_size = 10;
    else if (v < 0x30000000)
            mem_size = 11;
    else if (v < 0x40000000) // 1GB
            mem_size = 12;
    else if (v < 0x60000000) // 1.5GB
            mem_size = 13;
    else if (v < 0x80000000) // 2GB
            mem_size = 14;
    else if (v < 0xA8000000) // 2.5GB
            mem_size = 15;
    else if (v < 0xC0000000) // 3GB
            mem_size = 16;
    else
            mem_size = 17;
    ui->IDC_Z3FASTMEM->setValue(mem_size);

    mem_size = 0;
    switch (workprefs.gfxmem_size) {
    case 0x00000000: mem_size = 0; break;
    case 0x00100000: mem_size = 1; break;
    case 0x00200000: mem_size = 2; break;
    case 0x00400000: mem_size = 3; break;
    case 0x00800000: mem_size = 4; break;
    case 0x01000000: mem_size = 5; break;
    case 0x02000000: mem_size = 6; break;
    case 0x04000000: mem_size = 7; break;
    case 0x08000000: mem_size = 8; break;
    case 0x10000000: mem_size = 9; break;
    case 0x20000000: mem_size = 10; break;
    case 0x40000000: mem_size = 11; break;
    }
    //ui->IDC_P96MEM->setValue(mem_size);
    //ui->IDC_P96RAM->setValue(memsize_names[msi_gfx[mem_size]]);
}

void puae_MainWindow::enable_for_memorydlg ()
{
    int z3 = ! workprefs.address_space_24;
    int fast = workprefs.chipmem_size <= 0x200000;
    int rtg = workprefs.gfxmem_size; //&& full_property_sheet;
    int rtg2 = workprefs.gfxmem_size;

#ifndef AUTOCONFIG
    z3 = FALSE;
    fast = FALSE;
#endif

    ui->IDC_Z3FASTRAM->setEnabled(z3);
    ui->IDC_Z3FASTMEM->setEnabled(z3);
    ui->IDC_FASTMEM->setEnabled(fast);
    ui->IDC_FASTRAM->setEnabled(fast);
//    ui->IDC_FASTTEXT->setEnabled(fast);
//    ui->IDC_GFXCARDTEXT->setEnabled(z3);
    ui->IDC_P96RAM->setEnabled(z3);
    ui->IDC_P96MEM->setEnabled(z3);
    ui->IDC_MBRAM1->setEnabled(z3);
    ui->IDC_MBMEM1->setEnabled(z3);
    ui->IDC_MBRAM2->setEnabled(z3);
    ui->IDC_MBMEM2->setEnabled(z3);

    ui->IDC_RTG_8BIT->setEnabled(rtg);
    ui->IDC_RTG_16BIT->setEnabled(rtg);
    ui->IDC_RTG_24BIT->setEnabled(rtg);
    ui->IDC_RTG_32BIT->setEnabled(rtg);
    ui->IDC_RTG_MATCH_DEPTH->setEnabled(rtg2);
    ui->IDC_RTG_SCALE->setEnabled(rtg2);
    ui->IDC_RTG_SCALE_ALLOW->setEnabled(rtg2);
    ui->IDC_RTG_SCALE_ASPECTRATIO->setEnabled(rtg2);
    ui->IDC_RTG_VBLANKRATE->setEnabled(rtg2);
}

void puae_MainWindow::values_to_chipsetdlg ()
{
        switch (workprefs.chipset_mask) {
        case 0:
		ui->IDC_OCS->setChecked(true);
                break;
        case CSMASK_ECS_AGNUS:
		ui->IDC_ECS_AGNUS->setChecked(true);
                break;
        case CSMASK_ECS_DENISE:
		ui->IDC_ECS_DENISE->setChecked(true);
                break;
        case CSMASK_ECS_AGNUS | CSMASK_ECS_DENISE:
		ui->IDC_ECS->setChecked(true);
                break;
        case CSMASK_AGA:
        case CSMASK_ECS_AGNUS | CSMASK_ECS_DENISE | CSMASK_AGA:
		ui->IDC_AGA->setChecked(true);
                break;
        }
	
	ui->IDC_NTSC->setChecked(workprefs.ntscmode);
	ui->IDC_GENLOCK->setChecked(workprefs.genlock);
	ui->IDC_BLITIMM->setChecked(workprefs.immediate_blits);
	switch (workprefs.collision_level) {
	case 0:
		ui->IDC_COLLISION0->setChecked(true);
		break;
	case 1:
		ui->IDC_COLLISION1->setChecked(true);
		break;
	case 2:
		ui->IDC_COLLISION2->setChecked(true);
		break;
	case 3:
		ui->IDC_COLLISION3->setChecked(true);
		break;
	}
	ui->IDC_CYCLEEXACT->setChecked(workprefs.cpu_cycle_exact);
	int index = ui->IDC_CS_EXT->findData(workprefs.cs_compatible);
	ui->IDC_CS_EXT->setCurrentIndex(index);
}

void puae_MainWindow::values_from_chipsetdlg ()
{
        bool success = FALSE;
        int nn;
        bool n;

        workprefs.genlock = ui->IDC_GENLOCK->checkState();
        workprefs.immediate_blits = ui->IDC_BLITIMM->checkState();
        n = ui->IDC_CYCLEEXACT->checkState();
        if (workprefs.cpu_cycle_exact != n) {
                workprefs.cpu_cycle_exact = workprefs.blitter_cycle_exact = n;
                if (n) {
                        if (workprefs.cpu_model == 68000)
                                workprefs.cpu_compatible = 1;
                        if (workprefs.cpu_model <= 68020)
                                workprefs.m68k_speed = 0;
                        workprefs.immediate_blits = 0;
                        workprefs.gfx_framerate = 1;
                        workprefs.cachesize = 0;
                }   
        }
        workprefs.collision_level = ui->IDC_COLLISION0->isChecked() ? 0
                : ui->IDC_COLLISION1->isChecked() ? 1
                : ui->IDC_COLLISION2->isChecked() ? 2 : 3;
        workprefs.chipset_mask = ui->IDC_OCS->isChecked() ? 0
                : ui->IDC_ECS_AGNUS->isChecked() ? CSMASK_ECS_AGNUS
                : ui->IDC_ECS_DENISE->isChecked() ? CSMASK_ECS_DENISE
                : ui->IDC_ECS->isChecked() ? CSMASK_ECS_AGNUS | CSMASK_ECS_DENISE
                : CSMASK_AGA | CSMASK_ECS_AGNUS | CSMASK_ECS_DENISE;
        n = ui->IDC_NTSC->isChecked();
        if (workprefs.ntscmode != n) {
                workprefs.ntscmode = n;
        }
//TODO

}

void puae_MainWindow::values_to_chipsetdlg2 ()
{
        TCHAR txt[32];
        uae_u32 rev;

        switch(workprefs.cs_ciaatod) {
        case 0:                 
		ui->IDC_CS_CIAA_TOD1->setChecked(true);
                break;
        case 1:
		ui->IDC_CS_CIAA_TOD2->setChecked(true);
                break;
        case 2:
		ui->IDC_CS_CIAA_TOD3->setChecked(true);
                break;
        }

        switch(workprefs.cs_rtc) {
        case 0:
		ui->IDC_CS_RTC1->setChecked(true);
                break;
        case 1:
		ui->IDC_CS_RTC2->setChecked(true);
                break;
        case 2:
		ui->IDC_CS_RTC3->setChecked(true);
                break;
        }

        ui->IDC_CS_COMPATIBLE->setChecked(workprefs.cs_compatible);
        ui->IDC_CS_RESETWARNING->setChecked(workprefs.cs_resetwarning);
        ui->IDC_CS_NOEHB->setChecked(workprefs.cs_denisenoehb);
        ui->IDC_CS_DIPAGNUS->setChecked(workprefs.cs_dipagnus);
        ui->IDC_CS_KSMIRROR_E0->setChecked(workprefs.cs_ksmirror_e0);
        ui->IDC_CS_KSMIRROR_A8->setChecked(workprefs.cs_ksmirror_a8);
        ui->IDC_CS_CIAOVERLAY->setChecked(workprefs.cs_ciaoverlay);
        ui->IDC_CS_DF0IDHW->setChecked(workprefs.cs_df0idhw);
        ui->IDC_CS_CD32CD->setChecked(workprefs.cs_cd32cd);
        ui->IDC_CS_CD32C2P->setChecked(workprefs.cs_cd32c2p);
        ui->IDC_CS_CD32NVRAM->setChecked(workprefs.cs_cd32nvram);
        ui->IDC_CS_CDTVCD->setChecked(workprefs.cs_cdtvcd);
        ui->IDC_CS_CDTVRAM->setChecked(workprefs.cs_cdtvram);
        ui->IDC_CS_CDTVRAMEXP->setChecked(workprefs.cs_cdtvcard);
        ui->IDC_CS_A1000RAM->setChecked(workprefs.cs_a1000ram);
        ui->IDC_CS_RAMSEY->setChecked(workprefs.cs_ramseyrev >= 0);
        ui->IDC_CS_FATGARY->setChecked(workprefs.cs_fatgaryrev >= 0);
        ui->IDC_CS_AGNUS->setChecked(workprefs.cs_agnusrev >= 0);
        ui->IDC_CS_DENISE->setChecked(workprefs.cs_deniserev >= 0);
        ui->IDC_CS_DMAC->setChecked(workprefs.cs_mbdmac == 1);
        ui->IDC_CS_DMAC2->setChecked(workprefs.cs_mbdmac == 2);
        ui->IDC_CS_A2091->setChecked(workprefs.cs_a2091);
        ui->IDC_CS_A4091->setChecked(workprefs.cs_a4091);
        ui->IDC_CS_CDTVSCSI->setChecked(workprefs.cs_cdtvscsi);
  //      ui->IDC_CS_SCSIMODE->setChecked(workprefs.scsi == 2);
        ui->IDC_CS_PCMCIA->setChecked(workprefs.cs_pcmcia);
        ui->IDC_CS_SLOWISFAST->setChecked(workprefs.cs_slowmemisfast);
        ui->IDC_CS_IDE1->setChecked(workprefs.cs_ide > 0 && (workprefs.cs_ide & 1));
        ui->IDC_CS_IDE2->setChecked(workprefs.cs_ide > 0 && (workprefs.cs_ide & 2));
        txt[0] = 0;

        printf (txt, "%d", workprefs.cs_rtc_adjust);
        ui->IDC_CS_RTCADJUST->setText(txt);
        txt[0] = 0;
        if (workprefs.cs_fatgaryrev >= 0)
                printf (txt, "%02X", workprefs.cs_fatgaryrev);
        ui->IDC_CS_FATGARYREV->setText(txt);
        txt[0] = 0;
        if (workprefs.cs_ramseyrev >= 0)
                printf (txt, "%02X", workprefs.cs_ramseyrev);
        ui->IDC_CS_RAMSEYREV->setText(txt);
        txt[0] = 0;
        if (workprefs.cs_agnusrev >= 0) {
                rev = workprefs.cs_agnusrev;
                printf (txt, "%02X", rev);
        } else if (workprefs.cs_compatible) {
                rev = 0;
                if (workprefs.ntscmode)
                        rev |= 0x10;
                rev |= (workprefs.chipset_mask & CSMASK_AGA) ? 0x23 : 0;
                rev |= (currprefs.chipset_mask & CSMASK_ECS_AGNUS) ? 0x20 : 0;
                if (workprefs.chipmem_size > 1024 * 1024 && (workprefs.chipset_mask & CSMASK_ECS_AGNUS))
                        rev |= 0x21;
                printf (txt, "%02X", rev);
        }
        ui->IDC_CS_AGNUSREV->setText(txt);
        txt[0] = 0;
        if (workprefs.cs_deniserev >= 0) {
                rev = workprefs.cs_deniserev;
                printf (txt, "%01.1X", rev);
        } else if (workprefs.cs_compatible) {
                rev = 0xf;
                if (workprefs.chipset_mask & CSMASK_ECS_DENISE)
                        rev = 0xc;
                if (workprefs.chipset_mask & CSMASK_AGA)
                        rev = 0x8;
                printf (txt, "%01.1X", rev);
        }
        ui->IDC_CS_DENISEREV->setText(txt);
}

void puae_MainWindow::values_from_chipsetdlg2 ()
{
        TCHAR txt[32], *p;
        int v;

        workprefs.cs_compatible = ui->IDC_CS_COMPATIBLE->isChecked();
        workprefs.cs_resetwarning = ui->IDC_CS_RESETWARNING->isChecked();
        workprefs.cs_denisenoehb = ui->IDC_CS_NOEHB->isChecked();
        workprefs.cs_dipagnus = ui->IDC_CS_DIPAGNUS->isChecked();
        workprefs.cs_agnusbltbusybug = workprefs.cs_dipagnus;
        workprefs.cs_ksmirror_e0 = ui->IDC_CS_KSMIRROR_E0->isChecked();
        workprefs.cs_ksmirror_a8 = ui->IDC_CS_KSMIRROR_A8->isChecked();
        workprefs.cs_ciaoverlay = ui->IDC_CS_CIAOVERLAY->isChecked();
        workprefs.cs_df0idhw = ui->IDC_CS_DF0IDHW->isChecked();
        workprefs.cs_cd32cd = ui->IDC_CS_CD32CD->isChecked();
        workprefs.cs_cd32c2p = ui->IDC_CS_CD32C2P->isChecked();
        workprefs.cs_cd32nvram = ui->IDC_CS_CD32NVRAM->isChecked();
        workprefs.cs_cdtvcd = ui->IDC_CS_CDTVCD->isChecked();
        workprefs.cs_cdtvram = ui->IDC_CS_CDTVRAM->isChecked();
        workprefs.cs_cdtvcard = ui->IDC_CS_CDTVRAMEXP->isChecked() ? 64 : 0;
        workprefs.cs_a1000ram = ui->IDC_CS_A1000RAM->isChecked();
        workprefs.cs_ramseyrev = ui->IDC_CS_RAMSEY->isChecked() ? 0x0f : -1;
        workprefs.cs_fatgaryrev = ui->IDC_CS_FATGARY->isChecked() ? 0x00 : -1;
        workprefs.cs_mbdmac = ui->IDC_CS_DMAC->isChecked() ? 1 : 0;
        if (workprefs.cs_mbdmac == 0)
                workprefs.cs_mbdmac = ui->IDC_CS_DMAC2->isChecked() ? 2 : 0;
        workprefs.cs_a2091 = ui->IDC_CS_A2091->isChecked() ? 1 : 0;
        workprefs.cs_a4091 = ui->IDC_CS_A4091->isChecked() ? 1 : 0;

        workprefs.cs_cdtvscsi = ui->IDC_CS_CDTVSCSI->isChecked() ? 1 : 0;
        workprefs.cs_pcmcia = ui->IDC_CS_PCMCIA->isChecked() ? 1 : 0;
        workprefs.cs_slowmemisfast = ui->IDC_CS_SLOWISFAST->isChecked() ? 1 : 0;
        workprefs.cs_ide = ui->IDC_CS_IDE1->isChecked() ? 1 : (ui->IDC_CS_IDE2->isChecked() ? 2 : 0);
        workprefs.cs_ciaatod = ui->IDC_CS_CIAA_TOD1->isChecked() ? 0
                : (ui->IDC_CS_CIAA_TOD2->isChecked() ? 1 : 2);
        workprefs.cs_rtc = ui->IDC_CS_RTC1->isChecked() ? 0
                : (ui->IDC_CS_RTC2->isChecked() ? 1 : 2);

/*        if (workprefs.cs_rtc) {
                txt[0] = 0;
                SendDlgItemMessage (hDlg, IDC_CS_RTCADJUST, WM_GETTEXT, (WPARAM)sizeof (txt) / sizeof (TCHAR), (LPARAM)txt);
                workprefs.cs_rtc_adjust = _tstol(txt);
        }
        if (workprefs.cs_fatgaryrev >= 0) {
                txt[0] = 0;
                SendDlgItemMessage (hDlg, IDC_CS_FATGARYREV, WM_GETTEXT, (WPARAM)sizeof (txt) / sizeof (TCHAR), (LPARAM)txt);
                v = _tcstol (txt, &p, 16);
                if (v >= 0 && v <= 255)
                        workprefs.cs_fatgaryrev = v;
        }
        if (workprefs.cs_ramseyrev >= 0) {
                txt[0] = 0;
                SendDlgItemMessage (hDlg, IDC_CS_RAMSEYREV, WM_GETTEXT, (WPARAM)sizeof (txt) / sizeof (TCHAR), (LPARAM)txt);
                v = _tcstol (txt, &p, 16);
                if (v >= 0 && v <= 255)
                        workprefs.cs_ramseyrev = v;
        }
        if (workprefs.cs_agnusrev >= 0) {
                txt[0] = 0;
                SendDlgItemMessage (hDlg, IDC_CS_AGNUSREV, WM_GETTEXT, (WPARAM)sizeof (txt) / sizeof (TCHAR), (LPARAM)txt);
                v = _tcstol (txt, &p, 16);
                if (v >= 0 && v <= 255)
                        workprefs.cs_agnusrev = v;
        }
        if (workprefs.cs_deniserev >= 0) {
                txt[0] = 0;
                SendDlgItemMessage (hDlg, IDC_CS_DENISEREV, WM_GETTEXT, (WPARAM)sizeof (txt) / sizeof (TCHAR), (LPARAM)txt);
                v = _tcstol (txt, &p, 16);
                if (v >= 0 && v <= 15)
                        workprefs.cs_deniserev = v;
        }
*/

}

void puae_MainWindow::enable_for_chipsetdlg2 ()
{
        int e = workprefs.cs_compatible ? FALSE : TRUE;

        ui->IDC_CS_FATGARY->setEnabled(e);
        ui->IDC_CS_RAMSEY->setEnabled(e);
        ui->IDC_CS_AGNUS->setEnabled(e);
        ui->IDC_CS_DENISE->setEnabled(e);
        ui->IDC_CS_FATGARYREV->setEnabled(e);
        ui->IDC_CS_RAMSEYREV->setEnabled(e);
        ui->IDC_CS_AGNUSREV->setEnabled(e);
        ui->IDC_CS_DENISEREV->setEnabled(e);
        ui->IDC_CS_IDE1->setEnabled(e);
        ui->IDC_CS_IDE2->setEnabled(e);
        ui->IDC_CS_DMAC->setEnabled(e);
        ui->IDC_CS_DMAC2->setEnabled(e);
        ui->IDC_CS_A2091->setEnabled(e);
        ui->IDC_CS_A4091->setEnabled(e);
//        ShowWindow (GetDlgItem(hDlg, IDC_CS_SCSIMODE), SW_HIDE);
//        ui->IDC_CS_SCSIMODE, FALSE);
        ui->IDC_CS_CDTVSCSI->setEnabled(e);
        ui->IDC_CS_PCMCIA->setEnabled(e);
        ui->IDC_CS_SLOWISFAST->setEnabled(e);
        ui->IDC_CS_CD32CD->setEnabled(e);
        ui->IDC_CS_CD32NVRAM->setEnabled(e);
        ui->IDC_CS_CD32C2P->setEnabled(e);
        ui->IDC_CS_CDTVCD->setEnabled(e);
        ui->IDC_CS_CDTVRAM->setEnabled(e);
        ui->IDC_CS_CDTVRAMEXP->setEnabled(e);
        ui->IDC_CS_RESETWARNING->setEnabled(e);
        ui->IDC_CS_NOEHB->setEnabled(e);
        ui->IDC_CS_DIPAGNUS->setEnabled(e);
        ui->IDC_CS_KSMIRROR_E0->setEnabled(e);
        ui->IDC_CS_KSMIRROR_A8->setEnabled(e);
        ui->IDC_CS_CIAOVERLAY->setEnabled(e);
        ui->IDC_CS_A1000RAM->setEnabled(e);
        ui->IDC_CS_DF0IDHW->setEnabled(e);
        ui->IDC_CS_CIAA_TOD1->setEnabled(e);
        ui->IDC_CS_CIAA_TOD2->setEnabled(e);
        ui->IDC_CS_CIAA_TOD3->setEnabled(e);
        ui->IDC_CS_RTC1->setEnabled(e);
        ui->IDC_CS_RTC2->setEnabled(e);
        ui->IDC_CS_RTC3->setEnabled(e);
        ui->IDC_CS_RTCADJUST->setEnabled(e);

}

void puae_MainWindow::enable_for_chipsetdlg ()
{
        int enable = workprefs.cpu_cycle_exact ? FALSE : TRUE;
        
#if !defined (CPUEMU_12)
        ui->IDC_CYCLEEXACT->setEnabled(FALSE);
#endif
//        ui->IDC_FASTCOPPER->setEnabled(enable);
//        ui->IDC_GENLOCK->setEnabled(full_property_sheet);
        ui->IDC_BLITIMM->setEnabled(enable);
        if (enable == FALSE) {
                workprefs.immediate_blits = 0;
//		ui->IDC_FASTCOPPER->setEnabled(FALSE);
		ui->IDC_BLITIMM->setEnabled(FALSE);
        }
        ui->IDC_CS_EXT->setEnabled(workprefs.cs_compatible ? TRUE : FALSE);
}

void puae_MainWindow::enable_for_displaydlg ()
{
        int rtg = ! workprefs.address_space_24;
#ifndef PICASSO96
        rtg = FALSE;
#endif
        ui->IDC_SCREENMODE_RTG->setEnabled(rtg);
        ui->IDC_XCENTER->setEnabled(TRUE);
        ui->IDC_YCENTER->setEnabled(TRUE);
        ui->IDC_LM_SCANLINES->setEnabled(TRUE);
        ui->IDC_FRAMERATE2->setEnabled(!workprefs.gfx_avsync);
        ui->IDC_FRAMERATE->setEnabled(!workprefs.cpu_cycle_exact);
        ui->IDC_LORES->setEnabled(!workprefs.gfx_autoresolution);
        ui->IDC_LM_NORMAL->setEnabled(!workprefs.gfx_autoresolution);
        ui->IDC_LM_DOUBLED->setEnabled(!workprefs.gfx_autoresolution);
        ui->IDC_LM_SCANLINES->setEnabled(!workprefs.gfx_autoresolution);
}

void puae_MainWindow::enable_for_cpudlg ()
{
        bool enable = FALSE, jitenable = FALSE;
        bool cpu_based_enable = FALSE;
        bool fpu;

        /* These four items only get enabled when adjustable CPU style is enabled */
        ui->IDC_SPEED->setEnabled(workprefs.m68k_speed > 0);
        ui->IDC_COMPATIBLE24->setEnabled(workprefs.cpu_model == 68020);
        ui->IDC_CS_HOST->setEnabled(!workprefs.cpu_cycle_exact);
        ui->IDC_CS_68000->setEnabled(!workprefs.cpu_cycle_exact);
        ui->IDC_CS_ADJUSTABLE->setEnabled(!workprefs.cpu_cycle_exact);
        ui->IDC_CPUIDLE->setEnabled(workprefs.m68k_speed != 0 ? TRUE : FALSE);
#if !defined(CPUEMU_0) || defined(CPUEMU_68000_ONLY)
        ui->IDC_CPU1->setEnabled(FALSE);
        ui->IDC_CPU2->setEnabled(FALSE);
        ui->IDC_CPU3->setEnabled(FALSE);
        ui->IDC_CPU4->setEnabled(FALSE);
        ui->IDC_CPU5->setEnabled(FALSE);
#endif

        cpu_based_enable = workprefs.cpu_model >= 68020 && workprefs.address_space_24 == 0;

        jitenable = cpu_based_enable;
#ifndef JIT
        jitenable = FALSE;
#endif
        enable = jitenable && workprefs.cachesize;

        ui->IDC_TRUST0->setEnabled(enable);
        ui->IDC_TRUST1->setEnabled(enable);
        ui->IDC_HARDFLUSH->setEnabled(enable);
        ui->IDC_CONSTJUMP->setEnabled(enable);
        ui->IDC_JITFPU->setEnabled(enable);
        ui->IDC_NOFLAGS->setEnabled(enable);
//        ui->IDC_CS_CACHE_TEXT->setEnabled(enable);
        ui->IDC_CACHE->setEnabled(enable);
        ui->IDC_JITENABLE->setEnabled(jitenable);
        ui->IDC_COMPATIBLE->setEnabled(!workprefs.cpu_cycle_exact && !workprefs.cachesize);
        ui->IDC_COMPATIBLE_FPU->setEnabled(workprefs.fpu_model > 0);
        ui->IDC_CPU_FREQUENCY->setEnabled(workprefs.cpu_cycle_exact);
        ui->IDC_CPU_FREQUENCY2->setEnabled(workprefs.cpu_cycle_exact && !workprefs.cpu_clock_multiplier);

        fpu = TRUE;
        if (workprefs.cpu_model > 68030 || workprefs.cpu_compatible || workprefs.cpu_cycle_exact)
                fpu = FALSE;
        ui->IDC_FPU1->setEnabled(fpu);
        ui->IDC_FPU2->setEnabled(fpu);
        ui->IDC_FPU3->setEnabled(workprefs.cpu_model >= 68040);
        ui->IDC_MMUENABLE->setEnabled(workprefs.cpu_model == 68040 && workprefs.cachesize == 0);
}

void puae_MainWindow::values_to_cpudlg ()
{
        TCHAR cache[8] = "";
        int cpu;

//        ui->IDC_SPEED->setChecked( workprefs.m68k_speed <= 0 ? 1 : workprefs.m68k_speed / CYCLE_UNIT );
//        ui->IDC_CPUTEXT->setChecked(workprefs.m68k_speed <= 0 ? 1 : workprefs.m68k_speed / CYCLE_UNIT);
        ui->IDC_COMPATIBLE->setChecked(workprefs.cpu_compatible);
        ui->IDC_COMPATIBLE24->setChecked(workprefs.address_space_24);
        ui->IDC_COMPATIBLE_FPU->setChecked(workprefs.fpu_strict);
//	ui->IDC_CPUIDLE->setPOS ( workprefs.cpu_idle == 0 ? 0 : 12 - workprefs.cpu_idle / 15);
        cpu = (workprefs.cpu_model - 68000) / 10;
        if (cpu >= 5)
                cpu--;
	switch (cpu) {
	case 0:
        	ui->IDC_CPU0->setChecked(true);
	case 1:
        	ui->IDC_CPU1->setChecked(true);
	case 2:
        	ui->IDC_CPU2->setChecked(true);
	case 3:
        	ui->IDC_CPU3->setChecked(true);
	case 4:
        	ui->IDC_CPU4->setChecked(true);
	case 5:
        	ui->IDC_CPU5->setChecked(true);
	}

	switch (workprefs.fpu_model == 0 ? 0 : (workprefs.fpu_model == 68881 ? 1 : (workprefs.fpu_model == 68882 ? 2 : 3))) {
	case 0:
        	ui->IDC_FPU0->setChecked(true);
	case 1:
        	ui->IDC_FPU1->setChecked(true);
	case 2:
        	ui->IDC_FPU2->setChecked(true);
	case 3:
        	ui->IDC_FPU3->setChecked(true);
	}

/*        if (workprefs.m68k_speed == -1)
                CheckRadioButton(hDlg, IDC_CS_HOST, IDC_CS_ADJUSTABLE, IDC_CS_HOST);
        else if (workprefs.m68k_speed == 0)
                CheckRadioButton(hDlg, IDC_CS_HOST, IDC_CS_ADJUSTABLE, IDC_CS_68000);
        else
                CheckRadioButton(hDlg, IDC_CS_HOST, IDC_CS_ADJUSTABLE, IDC_CS_ADJUSTABLE);
*/
	switch (workprefs.comptrustbyte) {
	case 0:
		ui->IDC_TRUST0->setChecked(true);
	case 1:
		ui->IDC_TRUST1->setChecked(true);
	}

//        SendDlgItemMessage (hDlg, IDC_CACHE, TBM_SETPOS, TRUE, workprefs.cachesize / 1024);
        printf (cache, "%d MB", workprefs.cachesize / 1024 );
//        ui->IDC_CACHETEXT->setText(cache);

        ui->IDC_NOFLAGS->setChecked(workprefs.compnf);
        ui->IDC_JITFPU->setChecked(workprefs.compfpu);
        ui->IDC_HARDFLUSH->setChecked(workprefs.comp_hardflush);
        ui->IDC_CONSTJUMP->setChecked(workprefs.comp_constjump);
        ui->IDC_JITENABLE->setChecked(workprefs.cachesize > 0);
        ui->IDC_MMUENABLE->setChecked(workprefs.cpu_model == 68040 && workprefs.cachesize == 0 && workprefs.mmu_model == 68040);

        if (workprefs.cpu_cycle_exact) {
                if (workprefs.cpu_clock_multiplier) {
                        TCHAR txt[20];
                        double f = getcpufreq (workprefs.cpu_clock_multiplier);
                        printf (txt, "%.6f", f / 1000000.0);
			ui->IDC_CPU_FREQUENCY2->setText(txt);
                }
        } else {
		ui->IDC_CPU_FREQUENCY2->setText("");
        }
}

void puae_MainWindow::values_from_cpudlg ()
{
        int newcpu, newfpu, newtrust, oldcache, jitena, idx;
        static int cachesize_prev, trust_prev;

        workprefs.cpu_compatible = workprefs.cpu_cycle_exact | (ui->IDC_COMPATIBLE->isChecked() ? 1 : 0);
        workprefs.fpu_strict = ui->IDC_COMPATIBLE_FPU->isChecked() ? 1 : 0;
        workprefs.address_space_24 = ui->IDC_COMPATIBLE24->isChecked() ? 1 : 0;
/*        workprefs.m68k_speed = ui->IDC_CS_HOST->isChecked() ? -1
                : ui->IDC_CS_68000->isChecked() ? 0
                : SendMessage (GetDlgItem (hDlg, IDC_SPEED), TBM_GETPOS, 0, 0) * CYCLE_UNIT;*/
        workprefs.mmu_model = ui->IDC_MMUENABLE->isChecked() ? 68040 : 0;

        newcpu = ui->IDC_CPU0->isChecked() ? 68000
                : ui->IDC_CPU1->isChecked() ? 68010
                : ui->IDC_CPU2->isChecked() ? 68020
                : ui->IDC_CPU3->isChecked() ? 68030
                : ui->IDC_CPU4->isChecked() ? 68040
                : ui->IDC_CPU5->isChecked() ? 68060 : 0;
        newfpu = ui->IDC_FPU0->isChecked() ? 0
                : ui->IDC_FPU1->isChecked() ? 1
                : ui->IDC_FPU2->isChecked() ? 2
                : ui->IDC_FPU3->isChecked() ? 3 : 0;

        /* When switching away from 68000, disable 24 bit addressing.  */
        workprefs.cpu_model = newcpu;
        switch(newcpu)
        {
        case 68000:
        case 68010:
                workprefs.fpu_model = newfpu == 0 ? 0 : (newfpu == 2 ? 68882 : 68881);
                if (workprefs.cpu_compatible || workprefs.cpu_cycle_exact)
                        workprefs.fpu_model = 0;
                workprefs.address_space_24 = 1;
                if (newcpu == 0 && workprefs.cpu_cycle_exact)
                        workprefs.m68k_speed = 0;
                break;
        case 68020:
                workprefs.fpu_model = newfpu == 0 ? 0 : (newfpu == 2 ? 68882 : 68881);
                break;
        case 68030:
                workprefs.address_space_24 = 0;
                workprefs.fpu_model = newfpu == 0 ? 0 : (newfpu == 2 ? 68882 : 68881);
                break;
        case 68040:
                workprefs.fpu_model = newfpu ? 68040 : 0;
                workprefs.address_space_24 = 0;
                if (workprefs.fpu_model)
                        workprefs.fpu_model = 68040;
                break;
        case 68060:
                workprefs.fpu_model = newfpu ? 68060 : 0;
                workprefs.address_space_24 = 0;
                break;
        }

        newtrust = ui->IDC_TRUST0->isChecked() ? 0 : 1;
        workprefs.comptrustbyte = newtrust;
        workprefs.comptrustword = newtrust;
        workprefs.comptrustlong = newtrust;
        workprefs.comptrustnaddr= newtrust;

        workprefs.compnf            = ui->IDC_NOFLAGS->isChecked();
        workprefs.compfpu           = ui->IDC_JITFPU->isChecked();
        workprefs.comp_hardflush    = ui->IDC_HARDFLUSH->isChecked();
        workprefs.comp_constjump    = ui->IDC_CONSTJUMP->isChecked();

#ifdef JIT
        oldcache = workprefs.cachesize;
        jitena = ui->IDC_JITENABLE->isChecked() ? 1 : 0;
//        workprefs.cachesize = SendMessage (GetDlgItem (hDlg, IDC_CACHE), TBM_GETPOS, 0, 0) * 1024;
        if (!jitena) {
                cachesize_prev = workprefs.cachesize;
                trust_prev = workprefs.comptrustbyte;
                workprefs.cachesize = 0;
        } else if (jitena && !oldcache) {
                workprefs.cachesize = 8192;
                if (cachesize_prev) {
                        workprefs.cachesize = cachesize_prev;
                        workprefs.comptrustbyte = trust_prev;
                        workprefs.comptrustword = trust_prev;
                        workprefs.comptrustlong = trust_prev;
                        workprefs.comptrustnaddr = trust_prev;
                }
        }
        if (oldcache == 0 && candirect && workprefs.cachesize > 0)
                canbang = 1;
#endif
//        workprefs.cpu_idle = SendMessage (GetDlgItem (hDlg, IDC_CPUIDLE), TBM_GETPOS, 0, 0);
        if (workprefs.cpu_idle > 0)
                workprefs.cpu_idle = (12 - workprefs.cpu_idle) * 15;

        if (workprefs.cachesize > 0)
                workprefs.cpu_compatible = 0;
/*        if (pages[KICKSTART_ID])
                SendMessage (pages[KICKSTART_ID], WM_USER, 0, 0);
        if (pages[DISPLAY_ID])
                SendMessage (pages[DISPLAY_ID], WM_USER, 0, 0);
        if (pages[MEMORY_ID])
                SendMessage (pages[MEMORY_ID], WM_USER, 0, 0);*/

/*        idx = SendDlgItemMessage (hDlg, IDC_CPU_FREQUENCY, CB_GETCURSEL, 0, 0);
        if (idx != CB_ERR) {
                int m = workprefs.cpu_clock_multiplier;
                workprefs.cpu_frequency = 0;
                workprefs.cpu_clock_multiplier = 0;
                if (idx == 0)
                        workprefs.cpu_clock_multiplier = 2 << 8;
                if (idx == 1)
                        workprefs.cpu_clock_multiplier = 4 << 8;
                if (idx == 2)
                        workprefs.cpu_clock_multiplier = 8 << 8;
                if (idx == 3) {
                        TCHAR txt[20];
                        SendDlgItemMessage (hDlg, IDC_CPU_FREQUENCY2, WM_GETTEXT, (WPARAM)sizeof (txt) / sizeof (TCHAR), (LPARAM)txt);
                        workprefs.cpu_clock_multiplier = 0;
                        workprefs.cpu_frequency = _tstof (txt) * 1000000.0;
                        if (workprefs.cpu_frequency < 1 * 1000000)
                                workprefs.cpu_frequency = 0;
                        if (workprefs.cpu_frequency >= 99 * 1000000)
                                workprefs.cpu_frequency = 0;
                }
        }
*/
}

void puae_MainWindow::values_from_kickstartdlg () {
}

void puae_MainWindow::values_to_kickstartdlg () {
}

void puae_MainWindow::values_from_memorydlg () {
}

void puae_MainWindow::values_from_displaydlg () {
}

void puae_MainWindow::values_to_displaydlg () {
}

void puae_MainWindow::enable_for_sounddlg () {
}

void puae_MainWindow::values_from_sounddlg () {
}

void puae_MainWindow::values_to_sounddlg () {
}

void puae_MainWindow::enable_for_expansiondlg () {
}

void puae_MainWindow::values_to_expansiondlg () {
}

void puae_MainWindow::enable_for_miscdlg () {
}

void puae_MainWindow::values_to_miscdlg () {
}

void puae_MainWindow::enable_for_gameportsdlg () {
}

void puae_MainWindow::values_from_gameportsdlg () {
}

void puae_MainWindow::enable_for_inputdlg () {
}

void puae_MainWindow::values_from_inputdlg () {
}

void puae_MainWindow::values_to_inputdlg () {
}

void puae_MainWindow::init_portsdlg () {
}

void puae_MainWindow::enable_for_portsdlg () {
}

void puae_MainWindow::values_from_portsdlg () {
}

void puae_MainWindow::values_to_portsdlg () {
}
