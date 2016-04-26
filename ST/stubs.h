// Stub file to replace funcdefs.h during testing
// Generated: 22/05/2008 04:32:31

#ifdef USEGOTO
	#define NEXT goto next;
	#define IDECL(LabelName) LabelName :
#else
	#define NEXT
	#define IDECL(LabelName) void (LabelName)(void)
#endif

IDECL(InvalidCode)
{ printf("Function InvalidCode called\n");
 NEXT; 
}
IDECL(ori_b)
{ printf("Function ori_b called\n");
 NEXT; 
}
IDECL(ori_to_ccr)
{ printf("Function ori_to_ccr called\n");
 NEXT; 
}
IDECL(ori_w)
{ printf("Function ori_w called\n");
 NEXT; 
}
IDECL(ori_to_sr)
{ printf("Function ori_to_sr called\n");
 NEXT; 
}
IDECL(ori_l)
{ printf("Function ori_l called\n");
 NEXT; 
}
IDECL(btst_d)
{ printf("Function btst_d called\n");
 NEXT; 
}
IDECL(bchg_d)
{ printf("Function bchg_d called\n");
 NEXT; 
}
IDECL(bclr_d)
{ printf("Function bclr_d called\n");
 NEXT; 
}
IDECL(bset_d)
{ printf("Function bset_d called\n");
 NEXT; 
}
IDECL(movep_w_mr)
{ printf("Function movep_w_mr called\n");
 NEXT; 
}
IDECL(movep_l_mr)
{ printf("Function movep_l_mr called\n");
 NEXT; 
}
IDECL(movep_w_rm)
{ printf("Function movep_w_rm called\n");
 NEXT; 
}
IDECL(movep_l_rm)
{ printf("Function movep_l_rm called\n");
 NEXT; 
}
IDECL(andi_b)
{ printf("Function andi_b called\n");
 NEXT; 
}
IDECL(andi_w)
{ printf("Function andi_w called\n");
 NEXT; 
}
IDECL(andi_l)
{ printf("Function andi_l called\n");
 NEXT; 
}
IDECL(andi_to_ccr)
{ printf("Function andi_to_ccr called\n");
 NEXT; 
}
IDECL(andi_to_sr)
{ printf("Function andi_to_sr called\n");
 NEXT; 
}
IDECL(subi_b)
{ printf("Function subi_b called\n");
 NEXT; 
}
IDECL(subi_w)
{ printf("Function subi_w called\n");
 NEXT; 
}
IDECL(subi_l)
{ printf("Function subi_l called\n");
 NEXT; 
}
IDECL(addi_b)
{ printf("Function addi_b called\n");
 NEXT; 
}
IDECL(addi_w)
{ printf("Function addi_w called\n");
 NEXT; 
}
IDECL(addi_l)
{ printf("Function addi_l called\n");
 NEXT; 
}
IDECL(btst_s)
{ printf("Function btst_s called\n");
 NEXT; 
}
IDECL(bchg_s)
{ printf("Function bchg_s called\n");
 NEXT; 
}
IDECL(bclr_s)
{ printf("Function bclr_s called\n");
 NEXT; 
}
IDECL(bset_s)
{ printf("Function bset_s called\n");
 NEXT; 
}
IDECL(eori_b)
{ printf("Function eori_b called\n");
 NEXT; 
}
IDECL(eori_w)
{ printf("Function eori_w called\n");
 NEXT; 
}
IDECL(eori_l)
{ printf("Function eori_l called\n");
 NEXT; 
}
IDECL(eori_to_ccr)
{ printf("Function eori_to_ccr called\n");
 NEXT; 
}
IDECL(eori_to_sr)
{ printf("Function eori_to_sr called\n");
 NEXT; 
}
IDECL(cmpi_b)
{ printf("Function cmpi_b called\n");
 NEXT; 
}
IDECL(cmpi_w)
{ printf("Function cmpi_w called\n");
 NEXT; 
}
IDECL(cmpi_l)
{ printf("Function cmpi_l called\n");
 NEXT; 
}
IDECL(move_b)
{ printf("Function move_b called\n");
 NEXT; 
}
IDECL(move_b_from_dn)
{ printf("Function move_b_from_dn called\n");
 NEXT; 
}
IDECL(move_b_to_dn)
{ printf("Function move_b_to_dn called\n");
 NEXT; 
}
IDECL(move_b_reg)
{ printf("Function move_b_reg called\n");
 NEXT; 
}
IDECL(move_l)
{ printf("Function move_l called\n");
 NEXT; 
}
IDECL(move_l_from_dn)
{ printf("Function move_l_from_dn called\n");
 NEXT; 
}
IDECL(move_l_to_dn)
{ printf("Function move_l_to_dn called\n");
 NEXT; 
}
IDECL(move_l_reg)
{ printf("Function move_l_reg called\n");
 NEXT; 
}
IDECL(movea_l)
{ printf("Function movea_l called\n");
 NEXT; 
}
IDECL(movea_l_an)
{ printf("Function movea_l_an called\n");
 NEXT; 
}
IDECL(move_w)
{ printf("Function move_w called\n");
 NEXT; 
}
IDECL(move_w_from_dn)
{ printf("Function move_w_from_dn called\n");
 NEXT; 
}
IDECL(move_w_to_dn)
{ printf("Function move_w_to_dn called\n");
 NEXT; 
}
IDECL(move_w_reg)
{ printf("Function move_w_reg called\n");
 NEXT; 
}
IDECL(movea_w)
{ printf("Function movea_w called\n");
 NEXT; 
}
IDECL(negx_b)
{ printf("Function negx_b called\n");
 NEXT; 
}
IDECL(negx_w)
{ printf("Function negx_w called\n");
 NEXT; 
}
IDECL(negx_l)
{ printf("Function negx_l called\n");
 NEXT; 
}
IDECL(move_from_sr)
{ printf("Function move_from_sr called\n");
 NEXT; 
}
IDECL(chk)
{ printf("Function chk called\n");
 NEXT; 
}
IDECL(lea)
{ printf("Function lea called\n");
 NEXT; 
}
IDECL(clr_b)
{ printf("Function clr_b called\n");
 NEXT; 
}
IDECL(clr_w)
{ printf("Function clr_w called\n");
 NEXT; 
}
IDECL(clr_l)
{ printf("Function clr_l called\n");
 NEXT; 
}
IDECL(neg_b)
{ printf("Function neg_b called\n");
 NEXT; 
}
IDECL(neg_w)
{ printf("Function neg_w called\n");
 NEXT; 
}
IDECL(neg_l)
{ printf("Function neg_l called\n");
 NEXT; 
}
IDECL(move_to_ccr)
{ printf("Function move_to_ccr called\n");
 NEXT; 
}
IDECL(not_b)
{ printf("Function not_b called\n");
 NEXT; 
}
IDECL(not_w)
{ printf("Function not_w called\n");
 NEXT; 
}
IDECL(not_l)
{ printf("Function not_l called\n");
 NEXT; 
}
IDECL(move_to_sr)
{ printf("Function move_to_sr called\n");
 NEXT; 
}
IDECL(nbcd)
{ printf("Function nbcd called\n");
 NEXT; 
}
IDECL(pea)
{ printf("Function pea called\n");
 NEXT; 
}
IDECL(swap)
{ printf("Function swap called\n");
 NEXT; 
}
IDECL(movem_save_w)
{ printf("Function movem_save_w called\n");
 NEXT; 
}
IDECL(ext_w)
{ printf("Function ext_w called\n");
 NEXT; 
}
IDECL(movem_save_l)
{ printf("Function movem_save_l called\n");
 NEXT; 
}
IDECL(ext_l)
{ printf("Function ext_l called\n");
 NEXT; 
}
IDECL(tst_b)
{ printf("Function tst_b called\n");
 NEXT; 
}
IDECL(tst_w)
{ printf("Function tst_w called\n");
 NEXT; 
}
IDECL(tst_l)
{ printf("Function tst_l called\n");
 NEXT; 
}
IDECL(tas)
{ printf("Function tas called\n");
 NEXT; 
}
IDECL(illegal)
{ printf("Function illegal called\n");
 NEXT; 
}
IDECL(movem_load_w)
{ printf("Function movem_load_w called\n");
 NEXT; 
}
IDECL(movem_load_l)
{ printf("Function movem_load_l called\n");
 NEXT; 
}
IDECL(trap)
{ printf("Function trap called\n");
 NEXT; 
}
IDECL(trap0)
{ printf("Function trap0 called\n");
 NEXT; 
}
IDECL(trap1)
{ printf("Function trap1 called\n");
 NEXT; 
}
IDECL(trap2)
{ printf("Function trap2 called\n");
 NEXT; 
}
IDECL(trap3)
{ printf("Function trap3 called\n");
 NEXT; 
}
IDECL(link)
{ printf("Function link called\n");
 NEXT; 
}
IDECL(unlk)
{ printf("Function unlk called\n");
 NEXT; 
}
IDECL(move_to_usp)
{ printf("Function move_to_usp called\n");
 NEXT; 
}
IDECL(move_from_usp)
{ printf("Function move_from_usp called\n");
 NEXT; 
}
IDECL(reset)
{ printf("Function reset called\n");
 NEXT; 
}
IDECL(nop)
{ printf("Function nop called\n");
 NEXT; 
}
IDECL(stop)
{ printf("Function stop called\n");
 NEXT; 
}
IDECL(rte)
{ printf("Function rte called\n");
 NEXT; 
}
IDECL(rts)
{ printf("Function rts called\n");
 NEXT; 
}
IDECL(trapv)
{ printf("Function trapv called\n");
 NEXT; 
}
IDECL(rtr)
{ printf("Function rtr called\n");
 NEXT; 
}
IDECL(jsr)
{ printf("Function jsr called\n");
 NEXT; 
}
IDECL(jsr_displ)
{ printf("Function jsr_displ called\n");
 NEXT; 
}
IDECL(jmp)
{ printf("Function jmp called\n");
 NEXT; 
}
IDECL(addq_b)
{ printf("Function addq_b called\n");
 NEXT; 
}
IDECL(addq_w)
{ printf("Function addq_w called\n");
 NEXT; 
}
IDECL(addq_l)
{ printf("Function addq_l called\n");
 NEXT; 
}
IDECL(addq_an)
{ printf("Function addq_an called\n");
 NEXT; 
}
IDECL(addq_4_an)
{ printf("Function addq_4_an called\n");
 NEXT; 
}
IDECL(scc)
{ printf("Function scc called\n");
 NEXT; 
}
IDECL(st)
{ printf("Function st called\n");
 NEXT; 
}
IDECL(sf)
{ printf("Function sf called\n");
 NEXT; 
}
IDECL(dbcc)
{ printf("Function dbcc called\n");
 NEXT; 
}
IDECL(dbf)
{ printf("Function dbf called\n");
 NEXT; 
}
IDECL(subq_b)
{ printf("Function subq_b called\n");
 NEXT; 
}
IDECL(subq_w)
{ printf("Function subq_w called\n");
 NEXT; 
}
IDECL(subq_l)
{ printf("Function subq_l called\n");
 NEXT; 
}
IDECL(subq_an)
{ printf("Function subq_an called\n");
 NEXT; 
}
IDECL(subq_4_an)
{ printf("Function subq_4_an called\n");
 NEXT; 
}
IDECL(bcc_s)
{ printf("Function bcc_s called\n");
 NEXT; 
}
IDECL(bcc_bad)
{ printf("Function bcc_bad called\n");
 NEXT; 
}
IDECL(bccc_s)
{ printf("Function bccc_s called\n");
 NEXT; 
}
IDECL(bcs_s)
{ printf("Function bcs_s called\n");
 NEXT; 
}
IDECL(bne_s)
{ printf("Function bne_s called\n");
 NEXT; 
}
IDECL(beq_s)
{ printf("Function beq_s called\n");
 NEXT; 
}
IDECL(bpl_s)
{ printf("Function bpl_s called\n");
 NEXT; 
}
IDECL(bmi_s)
{ printf("Function bmi_s called\n");
 NEXT; 
}
IDECL(bge_s)
{ printf("Function bge_s called\n");
 NEXT; 
}
IDECL(blt_s)
{ printf("Function blt_s called\n");
 NEXT; 
}
IDECL(bgt_s)
{ printf("Function bgt_s called\n");
 NEXT; 
}
IDECL(ble_s)
{ printf("Function ble_s called\n");
 NEXT; 
}
IDECL(bcc_l)
{ printf("Function bcc_l called\n");
 NEXT; 
}
IDECL(bne_l)
{ printf("Function bne_l called\n");
 NEXT; 
}
IDECL(beq_l)
{ printf("Function beq_l called\n");
 NEXT; 
}
IDECL(bra_s)
{ printf("Function bra_s called\n");
 NEXT; 
}
IDECL(bra_l)
{ printf("Function bra_l called\n");
 NEXT; 
}
IDECL(bsr)
{ printf("Function bsr called\n");
 NEXT; 
}
IDECL(moveq)
{ printf("Function moveq called\n");
 NEXT; 
}
IDECL(or_b_dn)
{ printf("Function or_b_dn called\n");
 NEXT; 
}
IDECL(or_w_dn)
{ printf("Function or_w_dn called\n");
 NEXT; 
}
IDECL(or_l_dn)
{ printf("Function or_l_dn called\n");
 NEXT; 
}
IDECL(or_b_ea)
{ printf("Function or_b_ea called\n");
 NEXT; 
}
IDECL(or_w_ea)
{ printf("Function or_w_ea called\n");
 NEXT; 
}
IDECL(or_l_ea)
{ printf("Function or_l_ea called\n");
 NEXT; 
}
IDECL(divu)
{ printf("Function divu called\n");
 NEXT; 
}
IDECL(sbcd)
{ printf("Function sbcd called\n");
 NEXT; 
}
IDECL(divs)
{ printf("Function divs called\n");
 NEXT; 
}
IDECL(sub_b_dn)
{ printf("Function sub_b_dn called\n");
 NEXT; 
}
IDECL(sub_w_dn)
{ printf("Function sub_w_dn called\n");
 NEXT; 
}
IDECL(sub_l_dn)
{ printf("Function sub_l_dn called\n");
 NEXT; 
}
IDECL(sub_b_ea)
{ printf("Function sub_b_ea called\n");
 NEXT; 
}
IDECL(sub_w_ea)
{ printf("Function sub_w_ea called\n");
 NEXT; 
}
IDECL(sub_l_ea)
{ printf("Function sub_l_ea called\n");
 NEXT; 
}
IDECL(sub_w_an)
{ printf("Function sub_w_an called\n");
 NEXT; 
}
IDECL(sub_l_an)
{ printf("Function sub_l_an called\n");
 NEXT; 
}
IDECL(subx_b_r)
{ printf("Function subx_b_r called\n");
 NEXT; 
}
IDECL(subx_w_r)
{ printf("Function subx_w_r called\n");
 NEXT; 
}
IDECL(subx_l_r)
{ printf("Function subx_l_r called\n");
 NEXT; 
}
IDECL(subx_b_m)
{ printf("Function subx_b_m called\n");
 NEXT; 
}
IDECL(subx_w_m)
{ printf("Function subx_w_m called\n");
 NEXT; 
}
IDECL(subx_l_m)
{ printf("Function subx_l_m called\n");
 NEXT; 
}
IDECL(cmp_b)
{ printf("Function cmp_b called\n");
 NEXT; 
}
IDECL(cmp_w)
{ printf("Function cmp_w called\n");
 NEXT; 
}
IDECL(cmp_l)
{ printf("Function cmp_l called\n");
 NEXT; 
}
IDECL(cmp_b_dn)
{ printf("Function cmp_b_dn called\n");
 NEXT; 
}
IDECL(cmp_b_dan)
{ printf("Function cmp_b_dan called\n");
 NEXT; 
}
IDECL(cmp_w_dn)
{ printf("Function cmp_w_dn called\n");
 NEXT; 
}
IDECL(cmp_l_dn)
{ printf("Function cmp_l_dn called\n");
 NEXT; 
}
IDECL(cmpa_w)
{ printf("Function cmpa_w called\n");
 NEXT; 
}
IDECL(cmpa_l)
{ printf("Function cmpa_l called\n");
 NEXT; 
}
IDECL(cmpa_l_an)
{ printf("Function cmpa_l_an called\n");
 NEXT; 
}
IDECL(eor_b)
{ printf("Function eor_b called\n");
 NEXT; 
}
IDECL(eor_w)
{ printf("Function eor_w called\n");
 NEXT; 
}
IDECL(eor_l)
{ printf("Function eor_l called\n");
 NEXT; 
}
IDECL(cmpm_b)
{ printf("Function cmpm_b called\n");
 NEXT; 
}
IDECL(cmpm_w)
{ printf("Function cmpm_w called\n");
 NEXT; 
}
IDECL(cmpm_l)
{ printf("Function cmpm_l called\n");
 NEXT; 
}
IDECL(and_b_dn)
{ printf("Function and_b_dn called\n");
 NEXT; 
}
IDECL(and_w_dn)
{ printf("Function and_w_dn called\n");
 NEXT; 
}
IDECL(and_l_dn)
{ printf("Function and_l_dn called\n");
 NEXT; 
}
IDECL(and_l_dn_dn)
{ printf("Function and_l_dn_dn called\n");
 NEXT; 
}
IDECL(and_b_ea)
{ printf("Function and_b_ea called\n");
 NEXT; 
}
IDECL(and_w_ea)
{ printf("Function and_w_ea called\n");
 NEXT; 
}
IDECL(and_l_ea)
{ printf("Function and_l_ea called\n");
 NEXT; 
}
IDECL(mulu)
{ printf("Function mulu called\n");
 NEXT; 
}
IDECL(abcd)
{ printf("Function abcd called\n");
 NEXT; 
}
IDECL(exg_d)
{ printf("Function exg_d called\n");
 NEXT; 
}
IDECL(exg_a)
{ printf("Function exg_a called\n");
 NEXT; 
}
IDECL(exg_ad)
{ printf("Function exg_ad called\n");
 NEXT; 
}
IDECL(muls)
{ printf("Function muls called\n");
 NEXT; 
}
IDECL(add_b_dn)
{ printf("Function add_b_dn called\n");
 NEXT; 
}
IDECL(add_w_dn)
{ printf("Function add_w_dn called\n");
 NEXT; 
}
IDECL(add_l_dn)
{ printf("Function add_l_dn called\n");
 NEXT; 
}
IDECL(add_b_dn_dn)
{ printf("Function add_b_dn_dn called\n");
 NEXT; 
}
IDECL(add_w_dn_dn)
{ printf("Function add_w_dn_dn called\n");
 NEXT; 
}
IDECL(add_l_dn_dn)
{ printf("Function add_l_dn_dn called\n");
 NEXT; 
}
IDECL(add_b_ea)
{ printf("Function add_b_ea called\n");
 NEXT; 
}
IDECL(add_w_ea)
{ printf("Function add_w_ea called\n");
 NEXT; 
}
IDECL(add_l_ea)
{ printf("Function add_l_ea called\n");
 NEXT; 
}
IDECL(add_w_an)
{ printf("Function add_w_an called\n");
 NEXT; 
}
IDECL(add_l_an)
{ printf("Function add_l_an called\n");
 NEXT; 
}
IDECL(add_w_an_dn)
{ printf("Function add_w_an_dn called\n");
 NEXT; 
}
IDECL(add_l_an_dn)
{ printf("Function add_l_an_dn called\n");
 NEXT; 
}
IDECL(addx_b_r)
{ printf("Function addx_b_r called\n");
 NEXT; 
}
IDECL(addx_w_r)
{ printf("Function addx_w_r called\n");
 NEXT; 
}
IDECL(addx_l_r)
{ printf("Function addx_l_r called\n");
 NEXT; 
}
IDECL(addx_b_m)
{ printf("Function addx_b_m called\n");
 NEXT; 
}
IDECL(addx_w_m)
{ printf("Function addx_w_m called\n");
 NEXT; 
}
IDECL(addx_l_m)
{ printf("Function addx_l_m called\n");
 NEXT; 
}
IDECL(lsr_b_i)
{ printf("Function lsr_b_i called\n");
 NEXT; 
}
IDECL(lsl_b_i)
{ printf("Function lsl_b_i called\n");
 NEXT; 
}
IDECL(lsr1_b)
{ printf("Function lsr1_b called\n");
 NEXT; 
}
IDECL(lsl1_b)
{ printf("Function lsl1_b called\n");
 NEXT; 
}
IDECL(lsr_w_i)
{ printf("Function lsr_w_i called\n");
 NEXT; 
}
IDECL(lsl_w_i)
{ printf("Function lsl_w_i called\n");
 NEXT; 
}
IDECL(lsr1_w)
{ printf("Function lsr1_w called\n");
 NEXT; 
}
IDECL(lsl1_w)
{ printf("Function lsl1_w called\n");
 NEXT; 
}
IDECL(lsr_l_i)
{ printf("Function lsr_l_i called\n");
 NEXT; 
}
IDECL(lsl_l_i)
{ printf("Function lsl_l_i called\n");
 NEXT; 
}
IDECL(lsr1_l)
{ printf("Function lsr1_l called\n");
 NEXT; 
}
IDECL(lsl1_l)
{ printf("Function lsl1_l called\n");
 NEXT; 
}
IDECL(lsl2_l)
{ printf("Function lsl2_l called\n");
 NEXT; 
}
IDECL(lsr_b_r)
{ printf("Function lsr_b_r called\n");
 NEXT; 
}
IDECL(lsl_b_r)
{ printf("Function lsl_b_r called\n");
 NEXT; 
}
IDECL(lsr_w_r)
{ printf("Function lsr_w_r called\n");
 NEXT; 
}
IDECL(lsl_w_r)
{ printf("Function lsl_w_r called\n");
 NEXT; 
}
IDECL(lsr_l_r)
{ printf("Function lsr_l_r called\n");
 NEXT; 
}
IDECL(lsl_l_r)
{ printf("Function lsl_l_r called\n");
 NEXT; 
}
IDECL(asr_b_i)
{ printf("Function asr_b_i called\n");
 NEXT; 
}
IDECL(asl_b_i)
{ printf("Function asl_b_i called\n");
 NEXT; 
}
IDECL(asr_w_i)
{ printf("Function asr_w_i called\n");
 NEXT; 
}
IDECL(asl_w_i)
{ printf("Function asl_w_i called\n");
 NEXT; 
}
IDECL(asr_l_i)
{ printf("Function asr_l_i called\n");
 NEXT; 
}
IDECL(asl_l_i)
{ printf("Function asl_l_i called\n");
 NEXT; 
}
IDECL(asr_b_r)
{ printf("Function asr_b_r called\n");
 NEXT; 
}
IDECL(asl_b_r)
{ printf("Function asl_b_r called\n");
 NEXT; 
}
IDECL(asr_w_r)
{ printf("Function asr_w_r called\n");
 NEXT; 
}
IDECL(asl_w_r)
{ printf("Function asl_w_r called\n");
 NEXT; 
}
IDECL(asr_l_r)
{ printf("Function asr_l_r called\n");
 NEXT; 
}
IDECL(asl_l_r)
{ printf("Function asl_l_r called\n");
 NEXT; 
}
IDECL(roxr_b_i)
{ printf("Function roxr_b_i called\n");
 NEXT; 
}
IDECL(roxl_b_i)
{ printf("Function roxl_b_i called\n");
 NEXT; 
}
IDECL(roxr_w_i)
{ printf("Function roxr_w_i called\n");
 NEXT; 
}
IDECL(roxl_w_i)
{ printf("Function roxl_w_i called\n");
 NEXT; 
}
IDECL(roxr_l_i)
{ printf("Function roxr_l_i called\n");
 NEXT; 
}
IDECL(roxl_l_i)
{ printf("Function roxl_l_i called\n");
 NEXT; 
}
IDECL(roxr_b_r)
{ printf("Function roxr_b_r called\n");
 NEXT; 
}
IDECL(roxl_b_r)
{ printf("Function roxl_b_r called\n");
 NEXT; 
}
IDECL(roxr_w_r)
{ printf("Function roxr_w_r called\n");
 NEXT; 
}
IDECL(roxl_w_r)
{ printf("Function roxl_w_r called\n");
 NEXT; 
}
IDECL(roxr_l_r)
{ printf("Function roxr_l_r called\n");
 NEXT; 
}
IDECL(roxl_l_r)
{ printf("Function roxl_l_r called\n");
 NEXT; 
}
IDECL(ror_b_i)
{ printf("Function ror_b_i called\n");
 NEXT; 
}
IDECL(rol_b_i)
{ printf("Function rol_b_i called\n");
 NEXT; 
}
IDECL(ror_w_i)
{ printf("Function ror_w_i called\n");
 NEXT; 
}
IDECL(rol_w_i)
{ printf("Function rol_w_i called\n");
 NEXT; 
}
IDECL(ror_l_i)
{ printf("Function ror_l_i called\n");
 NEXT; 
}
IDECL(rol_l_i)
{ printf("Function rol_l_i called\n");
 NEXT; 
}
IDECL(ror_b_r)
{ printf("Function ror_b_r called\n");
 NEXT; 
}
IDECL(rol_b_r)
{ printf("Function rol_b_r called\n");
 NEXT; 
}
IDECL(ror_w_r)
{ printf("Function ror_w_r called\n");
 NEXT; 
}
IDECL(rol_w_r)
{ printf("Function rol_w_r called\n");
 NEXT; 
}
IDECL(ror_l_r)
{ printf("Function ror_l_r called\n");
 NEXT; 
}
IDECL(rol_l_r)
{ printf("Function rol_l_r called\n");
 NEXT; 
}
IDECL(asr_m)
{ printf("Function asr_m called\n");
 NEXT; 
}
IDECL(asl_m)
{ printf("Function asl_m called\n");
 NEXT; 
}
IDECL(lsr_m)
{ printf("Function lsr_m called\n");
 NEXT; 
}
IDECL(lsl_m)
{ printf("Function lsl_m called\n");
 NEXT; 
}
IDECL(roxr_m)
{ printf("Function roxr_m called\n");
 NEXT; 
}
IDECL(roxl_m)
{ printf("Function roxl_m called\n");
 NEXT; 
}
IDECL(ror_m)
{ printf("Function ror_m called\n");
 NEXT; 
}
IDECL(rol_m)
{ printf("Function rol_m called\n");
 NEXT; 
}
IDECL(code1010)
{ printf("Function code1010 called\n");
 NEXT; 
}
IDECL(InitROM)
{ printf("Function InitROM called\n");
 NEXT; 
}
IDECL(MdvIO)
{ printf("Function MdvIO called\n");
 NEXT; 
}
IDECL(MdvOpen)
{ printf("Function MdvOpen called\n");
 NEXT; 
}
IDECL(MdvClose)
{ printf("Function MdvClose called\n");
 NEXT; 
}
IDECL(MdvSlaving)
{ printf("Function MdvSlaving called\n");
 NEXT; 
}
IDECL(MdvFormat)
{ printf("Function MdvFormat called\n");
 NEXT; 
}
IDECL(SerIO)
{ printf("Function SerIO called\n");
 NEXT; 
}
IDECL(SerOpen)
{ printf("Function SerOpen called\n");
 NEXT; 
}
IDECL(SerClose)
{ printf("Function SerClose called\n");
 NEXT; 
}
IDECL(SchedulerCmd)
{ printf("Function SchedulerCmd called\n");
 NEXT; 
}
IDECL(DrvIO)
{ printf("Function DrvIO called\n");
 NEXT; 
}
IDECL(DrvOpen)
{ printf("Function DrvOpen called\n");
 NEXT; 
}
IDECL(DrvClose)
{ printf("Function DrvClose called\n");
 NEXT; 
}
IDECL(KbdCmd)
{ printf("Function KbdCmd called\n");
 NEXT; 
}
IDECL(PollCmd)
{ printf("Function PollCmd called\n");
 NEXT; 
}
IDECL(KBencCmd)
{ printf("Function KBencCmd called\n");
 NEXT; 
}
IDECL(BASEXTCmd)
{ printf("Function BASEXTCmd called\n");
 NEXT; 
}
IDECL(devpefio_cmd)
{ printf("Function devpefio_cmd called\n");
 NEXT; 
}
IDECL(devpefo_cmd)
{ printf("Function devpefo_cmd called\n");
 NEXT; 
}
IDECL(UseIPC)
{ printf("Function UseIPC called\n");
 NEXT; 
}
IDECL(ReadIPC)
{ printf("Function ReadIPC called\n");
 NEXT; 
}
IDECL(WriteIPC)
{ printf("Function WriteIPC called\n");
 NEXT; 
}
IDECL(FastStartup)
{ printf("Function FastStartup called\n");
 NEXT; 
}
IDECL(QL_KeyTrans)
{ printf("Function QL_KeyTrans called\n");
 NEXT; 
}
IDECL(code1111)
{ printf("Function code1111 called\n");
 NEXT; 
}

typedef unsigned char Cond;
#define true 1
#define false 0

