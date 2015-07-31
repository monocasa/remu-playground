[Bits 64]

;
; All hypercalls route to the same caller stub.  Calling convention for 
;    hypercalls matches amd64 SYSV ABI calling convention, so we dont'
;    have to fixup anything.
;

; extern void hypercall_v_i(HypercallType call, int arg);
[GLOBAL _ZN2os5board6kvmvmm13hypercall_v_iENS1_13HypercallTypeEi]
_ZN2os5board6kvmvmm13hypercall_v_iENS1_13HypercallTypeEi:

; extern void hypercall_v_v(HypercallType call);
[GLOBAL _ZN2os5board6kvmvmm13hypercall_v_vENS1_13HypercallTypeE]
_ZN2os5board6kvmvmm13hypercall_v_vENS1_13HypercallTypeE:

; extern ssize_t hypercall_S(HypercallType call, const char *str, void *buffer, size_t size);
[GLOBAL _ZN2os5board6kvmvmm11hypercall_SENS1_13HypercallTypeEPKcPvm]
_ZN2os5board6kvmvmm11hypercall_SENS1_13HypercallTypeEPKcPvm:

	out    0, eax
	ret

