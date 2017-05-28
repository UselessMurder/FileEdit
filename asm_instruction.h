#ifndef ASM_INSTRUCTION_H
#define ASM_INSTRUCTION_H

//common
#define ret asm volatile (".intel_syntax;" "ret;" ".att_syntax;" : : :)
#define call(value) asm volatile (".intel_syntax;" "call %0;" ".att_syntax;" : : "r"(value) :)
#define jmp(value) asm volatile (".intel_syntax;" "jmp %0;" ".att_syntax;" : : "r"(value) :)

#define get_es(value) asm volatile(".intel_syntax;" "mov %0, es;" ".att_syntax;" : "=r"(value) : :)
#define get_cs(value) asm volatile(".intel_syntax;" "mov %0, cs;" ".att_syntax;" : "=r"(value) : :)
#define get_ss(value) asm volatile(".intel_syntax;" "mov %0, ss;" ".att_syntax;" : "=r"(value) : :)
#define get_ds(value) asm volatile(".intel_syntax;" "mov %0, ds;" ".att_syntax;" : "=r"(value) : :)
#define get_fs(value) asm volatile(".intel_syntax;" "mov %0, fs;" ".att_syntax;" : "=r"(value) : :)
#define get_gs(value) asm volatile(".intel_syntax;" "mov %0, gs;" ".att_syntax;" : "=r"(value) : :)

#define set_es(value) asm volatile(".intel_syntax;" "mov es, %0;" ".att_syntax;" : : "r"(value) :)
#define set_cs(value) asm volatile(".intel_syntax;" "mov cs, %0;" ".att_syntax;" : : "r"(value) :)
#define set_ss(value) asm volatile(".intel_syntax;" "mov ss, %0;" ".att_syntax;" : : "r"(value) :)
#define set_ds(value) asm volatile(".intel_syntax;" "mov ds, %0;" ".att_syntax;" : : "r"(value) :)
#define set_fs(value) asm volatile(".intel_syntax;" "mov fs, %0;" ".att_syntax;" : : "r"(value) :)
#define set_gs(value) asm volatile(".intel_syntax;" "mov gs, %0;" ".att_syntax;" : : "r"(value) :)


//x86
#define push_32(value) asm volatile (".intel_syntax;" "mov eax, %0;" "push eax;" ".att_syntax;" : : "r"(value) : "%eax")
#define pop_32(value) asm volatile (".intel_syntax;" "pop eax;" "mov %0, eax;" ".att_syntax;" : "=r"(value) : : "%eax")

#define get_eax(value) asm volatile(".intel_syntax;" "mov %0, eax;" ".att_syntax;" : "=r"(value) : :)
#define get_ebx(value) asm volatile(".intel_syntax;" "mov %0, ebx;" ".att_syntax;" : "=r"(value) : :)
#define get_ecx(value) asm volatile(".intel_syntax;" "mov %0, ecx;" ".att_syntax;" : "=r"(value) : :)
#define get_edx(value) asm volatile(".intel_syntax;" "mov %0, edx;" ".att_syntax;" : "=r"(value) : :)
#define get_esp(value) asm volatile(".intel_syntax;" "mov %0, esp;" ".att_syntax;" : "=r"(value) : :)
#define get_ebp(value) asm volatile(".intel_syntax;" "mov %0, ebp;" ".att_syntax;" : "=r"(value) : :)
#define get_esi(value) asm volatile(".intel_syntax;" "mov %0, esi;" ".att_syntax;" : "=r"(value) : :)
#define get_edi(value) asm volatile(".intel_syntax;" "mov %0, edi;" ".att_syntax;" : "=r"(value) : :)

#define set_eax(value) asm volatile(".intel_syntax;" "mov eax, %0;" ".att_syntax;" : : "r"(value) :)
#define set_ebx(value) asm volatile(".intel_syntax;" "mov ebx, %0;" ".att_syntax;" : : "r"(value) :)
#define set_ecx(value) asm volatile(".intel_syntax;" "mov ecx, %0;" ".att_syntax;" : : "r"(value) :)
#define set_edx(value) asm volatile(".intel_syntax;" "mov edx, %0;" ".att_syntax;" : : "r"(value) :)
#define set_esp(value) asm volatile(".intel_syntax;" "mov esp, %0;" ".att_syntax;" : : "r"(value) :)
#define set_ebp(value) asm volatile(".intel_syntax;" "mov ebp, %0;" ".att_syntax;" : : "r"(value) :)
#define set_esi(value) asm volatile(".intel_syntax;" "mov esi, %0;" ".att_syntax;" : : "r"(value) :)
#define set_edi(value) asm volatile(".intel_syntax;" "mov edi, %0;" ".att_syntax;" : : "r"(value) :)

#define get_efl(value) asm volatile(".intel_syntax;" "mov %0, efl;" ".att_syntax;" : "=r"(value) : :)
#define set_efl(value) asm volatile(".intel_syntax;" "mov efl, %0;" ".att_syntax;" : : "r"(value) :)

#define push_eax asm volatile(".intel_syntax;" "push eax;" ".att_syntax;" : : :)

#define pop_eax asm volatile(".intel_syntax;" "pop eax;" ".att_syntax;" : : :)


//16bit
#define push_16(value) asm volatile (".intel_syntax;" "mov ax, %0;" "push ax;" ".att_syntax;" : : "r"(value) : "%ax")
#define pop_16(value) asm volatile (".intel_syntax;" "pop ax;" "mov %0, ax;" ".att_syntax;" : "=r"(value) : : "%ax")

#define get_ax(value) asm volatile(".intel_syntax;" "mov %0, ax;" ".att_syntax;" : "=r"(value) : :)
#define get_bx(value) asm volatile(".intel_syntax;" "mov %0, bx;" ".att_syntax;" : "=r"(value) : :)
#define get_cx(value) asm volatile(".intel_syntax;" "mov %0, cx;" ".att_syntax;" : "=r"(value) : :)
#define get_dx(value) asm volatile(".intel_syntax;" "mov %0, dx;" ".att_syntax;" : "=r"(value) : :)

#define set_ax(value) asm volatile(".intel_syntax;" "mov ax, %0;" ".att_syntax;" : : "r"(value) :)
#define set_bx(value) asm volatile(".intel_syntax;" "mov bx, %0;" ".att_syntax;" : : "r"(value) :)
#define set_cx(value) asm volatile(".intel_syntax;" "mov cx, %0;" ".att_syntax;" : : "r"(value) :)
#define set_dx(value) asm volatile(".intel_syntax;" "mov dx, %0;" ".att_syntax;" : : "r"(value) :)

#define get_al(value) asm volatile(".intel_syntax;" "mov %0, al;" ".att_syntax;" : "=r"(value) : :)
#define get_bl(value) asm volatile(".intel_syntax;" "mov %0, bl;" ".att_syntax;" : "=r"(value) : :)
#define get_cl(value) asm volatile(".intel_syntax;" "mov %0, cl;" ".att_syntax;" : "=r"(value) : :)
#define get_dl(value) asm volatile(".intel_syntax;" "mov %0, dl;" ".att_syntax;" : "=r"(value) : :)

#define set_al(value) asm volatile(".intel_syntax;" "mov al, %0;" ".att_syntax;" : : "r"(value) :)
#define set_bl(value) asm volatile(".intel_syntax;" "mov bl, %0;" ".att_syntax;" : : "r"(value) :)
#define set_cl(value) asm volatile(".intel_syntax;" "mov cl, %0;" ".att_syntax;" : : "r"(value) :)
#define set_dl(value) asm volatile(".intel_syntax;" "mov dl, %0;" ".att_syntax;" : : "r"(value) :)

#define get_ah(value) asm volatile(".intel_syntax;" "mov %0, ah;" ".att_syntax;" : "=r"(value) : :)
#define get_bh(value) asm volatile(".intel_syntax;" "mov %0, bh;" ".att_syntax;" : "=r"(value) : :)
#define get_ch(value) asm volatile(".intel_syntax;" "mov %0, ch;" ".att_syntax;" : "=r"(value) : :)
#define get_dh(value) asm volatile(".intel_syntax;" "mov %0, dh;" ".att_syntax;" : "=r"(value) : :)

#define set_ah(value) asm volatile(".intel_syntax;" "mov ah, %0;" ".att_syntax;" : : "r"(value) :)
#define set_bh(value) asm volatile(".intel_syntax;" "mov bh, %0;" ".att_syntax;" : : "r"(value) :)
#define set_ch(value) asm volatile(".intel_syntax;" "mov ch, %0;" ".att_syntax;" : : "r"(value) :)
#define set_dh(value) asm volatile(".intel_syntax;" "mov dh, %0;" ".att_syntax;" : : "r"(value) :)

// other
union r_x {
    unsigned long long all;
    unsigned long part[2];
};

union e_x {
    unsigned long all;
    unsigned short int part[2];
};

union __x {
    unsigned short int all;
    unsigned char part[2];
};

#endif // ASM_INSTRUCTION_H
