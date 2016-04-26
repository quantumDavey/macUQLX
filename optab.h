// Header file for Instruction Table setup
// Generated: 22/05/2008 04:32:31
// DST 2008

typedef struct { void (*f)(void); char *m; int s; int d; int x; } OT;

typedef void (*OX)(void);

OT optable[65536];

#define Z 296

OX OpX[Z]={	&InvalidCode,      &ori_b,            &ori_to_ccr,       &ori_w,            &ori_to_sr,        &ori_l,
			&btst_d,           &bchg_d,           &bclr_d,           &bset_d,           &movep_w_mr,       &movep_l_mr,
			&movep_w_rm,       &movep_l_rm,       &andi_b,           &andi_w,           &andi_l,           &andi_to_ccr,
			&andi_to_sr,       &subi_b,           &subi_w,           &subi_l,           &addi_b,           &addi_w,
			&addi_l,           &btst_s,           &bchg_s,           &bclr_s,           &bset_s,           &eori_b,
			&eori_w,           &eori_l,           &eori_to_ccr,      &eori_to_sr,       &cmpi_b,           &cmpi_w,
			&cmpi_l,           &move_b,           &move_b_from_dn,   &move_b_to_dn,     &move_b_reg,       &move_l,
			&move_l_from_dn,   &move_l_to_dn,     &move_l_reg,       &movea_l,          &movea_l_an,       &move_w,
			&move_w_from_dn,   &move_w_to_dn,     &move_w_reg,       &movea_w,          &negx_b,           &negx_w,
			&negx_l,           &move_from_sr,     &chk,              &lea,              &clr_b,            &clr_w,
			&clr_l,            &neg_b,            &neg_w,            &neg_l,            &move_to_ccr,      &not_b,
			&not_w,            &not_l,            &move_to_sr,       &nbcd,             &pea,              &swap,
			&movem_save_w,     &ext_w,            &movem_save_l,     &ext_l,            &tst_b,            &tst_w,
			&tst_l,            &tas,              &illegal,          &movem_load_w,     &movem_load_l,     &trap,
			&trap0,            &trap1,            &trap2,            &trap3,            &link,             &unlk,
			&move_to_usp,      &move_from_usp,    &reset,            &nop,              &stop,             &rte,
			&rts,              &trapv,            &rtr,              &jsr,              &jsr_displ,        &jmp,
			&addq_b,           &addq_w,           &addq_l,           &addq_an,          &addq_4_an,        &scc,
			&st,               &sf,               &dbcc,             &dbf,              &subq_b,           &subq_w,
			&subq_l,           &subq_an,          &subq_4_an,        &bcc_s,            &bcc_bad,          &bccc_s,
			&bcs_s,            &bne_s,            &beq_s,            &bpl_s,            &bmi_s,            &bge_s,
			&blt_s,            &bgt_s,            &ble_s,            &bcc_l,            &bne_l,            &beq_l,
			&bra_s,            &bra_l,            &bsr,              &moveq,            &or_b_dn,          &or_w_dn,
			&or_l_dn,          &or_b_ea,          &or_w_ea,          &or_l_ea,          &divu,             &sbcd,
			&divs,             &sub_b_dn,         &sub_w_dn,         &sub_l_dn,         &sub_b_ea,         &sub_w_ea,
			&sub_l_ea,         &sub_w_an,         &sub_l_an,         &subx_b_r,         &subx_w_r,         &subx_l_r,
			&subx_b_m,         &subx_w_m,         &subx_l_m,         &cmp_b,            &cmp_w,            &cmp_l,
			&cmp_b_dn,         &cmp_b_dan,        &cmp_w_dn,         &cmp_l_dn,         &cmpa_w,           &cmpa_l,
			&cmpa_l_an,        &eor_b,            &eor_w,            &eor_l,            &cmpm_b,           &cmpm_w,
			&cmpm_l,           &and_b_dn,         &and_w_dn,         &and_l_dn,         &and_l_dn_dn,      &and_b_ea,
			&and_w_ea,         &and_l_ea,         &mulu,             &abcd,             &exg_d,            &exg_a,
			&exg_ad,           &muls,             &add_b_dn,         &add_w_dn,         &add_l_dn,         &add_b_dn_dn,
			&add_w_dn_dn,      &add_l_dn_dn,      &add_b_ea,         &add_w_ea,         &add_l_ea,         &add_w_an,
			&add_l_an,         &add_w_an_dn,      &add_l_an_dn,      &addx_b_r,         &addx_w_r,         &addx_l_r,
			&addx_b_m,         &addx_w_m,         &addx_l_m,         &lsr_b_i,          &lsl_b_i,          &lsr1_b,
			&lsl1_b,           &lsr_w_i,          &lsl_w_i,          &lsr1_w,           &lsl1_w,           &lsr_l_i,
			&lsl_l_i,          &lsr1_l,           &lsl1_l,           &lsl2_l,           &lsr_b_r,          &lsl_b_r,
			&lsr_w_r,          &lsl_w_r,          &lsr_l_r,          &lsl_l_r,          &asr_b_i,          &asl_b_i,
			&asr_w_i,          &asl_w_i,          &asr_l_i,          &asl_l_i,          &asr_b_r,          &asl_b_r,
			&asr_w_r,          &asl_w_r,          &asr_l_r,          &asl_l_r,          &roxr_b_i,         &roxl_b_i,
			&roxr_w_i,         &roxl_w_i,         &roxr_l_i,         &roxl_l_i,         &roxr_b_r,         &roxl_b_r,
			&roxr_w_r,         &roxl_w_r,         &roxr_l_r,         &roxl_l_r,         &ror_b_i,          &rol_b_i,
			&ror_w_i,          &rol_w_i,          &ror_l_i,          &rol_l_i,          &ror_b_r,          &rol_b_r,
			&ror_w_r,          &rol_w_r,          &ror_l_r,          &rol_l_r,          &asr_m,            &asl_m,
			&lsr_m,            &lsl_m,            &roxr_m,           &roxl_m,           &ror_m,            &rol_m,
			&code1010,         &InitROM,          &MdvIO,            &MdvOpen,          &MdvClose,         &MdvSlaving,
			&MdvFormat,        &SerIO,            &SerOpen,          &SerClose,         &SchedulerCmd,     &DrvIO,
			&DrvOpen,          &DrvClose,         &KbdCmd,           &PollCmd,          &KBencCmd,         &BASEXTCmd,
			&devpefio_cmd,     &devpefo_cmd,      &UseIPC,           &ReadIPC,          &WriteIPC,         &FastStartup,
			&QL_KeyTrans,      &code1111	};


char OpMnem[]="\0Invalid\0ori.b\0ori\0ori.w\0ori.l\0btst\0bchg\0bclr\0bset\0movep.w\0movep.l\0andi.b\0andi.w\0andi.l\0andi\0subi.b\0subi.w\0subi.l\0addi.b\0addi.w\0addi.l\0btst.s\0bchg.s\0bclr.s\0bset.s\0eori.b\0eori.w\0eori.l\0eori\0cmpi.b\0cmpi.w\0cmpi.l\0move.b\0move.l\0movea.l\0move.w\0movea.w\0negx.b\0negx.w\0negx.l\0move\0chk.w\0lea\0clr.b\0clr.w\0clr.l\0neg.b\0neg.w\0neg.l\0not.b\0not.w\0not.l\0nbcd\0pea\0swap\0movem.w\0ext.w\0movem.l\0ext.l\0tst.b\0tst.w\0tst.l\0tas\0illegal\0trap\0trap#0\0trap#1\0trap#2\0trap#3\0link\0unlk\0reset\0nop\0stop\0rte\0rts\0trapv\0rtr\0jsr\0jmp\0addq.b\0addq.w\0addq.l\0sCC\0st\0sf\0shi\0sls\0scc\0scs\0sne\0seq\0svc\0svs\0spl\0smi\0sge\0slt\0sgt\0sle\0dbcc\0dbt\0dbf\0dbhi\0dbls\0dbcs\0dbne\0dbeq\0dbvc\0dbvs\0dbpl\0dbmi\0dbge\0dblt\0dbgt\0dble\0subq.b\0subq.w\0subq.l\0bcc.s\0bcc_BAD\0bhi.s\0bls.s\0bccc.s\0bcs.s\0bne.s\0beq.s\0bvc.s\0bvs.s\0bpl.s\0bmi.s\0bge.s\0blt.s\0bgt.s\0ble.s\0bcc.l\0bne.l\0beq.l\0bra.s\0bra.l\0bsr\0moveq\0or.b\0or.w\0or.l\0divu.w\0sbcd\0divs.w\0sub.b\0sub.w\0sub.l\0suba.w\0suba.l\0subx.b\0subx.w\0subx.l\0cmp.b\0cmp.w\0cmp.l\0cmpa.w\0cmpa.l\0eor.b\0eor.w\0eor.l\0cmpm.b\0cmpm.w\0cmpm.l\0and.b\0and.w\0and.l\0mulu.w\0abcd\0exg_d\0exg_a\0exg_ad\0muls.w\0add.b\0add.w\0add.l\0adda.w\0adda.l\0addx.b\0addx.w\0addx.l\0lsr.b\0lsl.b\0lsr.w\0lsl.w\0lsr.l\0lsl.l\0asr.b\0asl.b\0asr.w\0asl.w\0asr.l\0asl.l\0roxr.b\0roxl.b\0roxr.w\0roxl.w\0roxr.l\0roxl.l\0ror.b\0rol.b\0ror.w\0rol.w\0ror.l\0rol.l\0asr\0asl\0lsr\0lsl\0roxr\0roxl\0ror\0rol\0a_line\0ROMINI\0MDVIO\0MDVO\0MDVC\0MDVSL\0MDVFO\0OSERIO\0OSERO\0OSERC\0SCHED\0DEVIO\0DEVO\0DEVC\0MIPC\0POLL\0KBENC\0BASXT\0DPF\0DPEFO\0IPC\0IPCR\0IPCW\0FSTART\0KYTRNS\0f_line\0\0";

