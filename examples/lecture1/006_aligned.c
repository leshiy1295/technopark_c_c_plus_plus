#include <stdio.h>
#include <stdint.h>

typedef int __attribute__((aligned(256))) al256int_t;

uint64_t global_simple_var;
uint64_t global_aligned_var __attribute__((aligned(128)));
al256int_t global_typed_aligned_var;

void print_align_results(uint64_t *simple_var, uint64_t *aligned_var, al256int_t *typed_aligned_var) {
    printf("simple: %p, sizeof: %ld, modulus to 128: %ld\n", simple_var, sizeof *simple_var, (uint64_t)simple_var % 128);
    printf("aligned: %p, sizeof: %ld, modulus to 128: %ld\n", aligned_var, sizeof *aligned_var, (uint64_t)aligned_var % 128);
    printf("typed aligned: %p, sizeof: %ld, modulus to 256: %ld\n", typed_aligned_var, sizeof *typed_aligned_var, (uint64_t)typed_aligned_var % 256);
}

int main(int argc, char *argv[]) {
    uint64_t simple_var;
    uint64_t aligned_var __attribute__((aligned(128)));
    al256int_t typed_aligned_var;

    static uint64_t static_simple_var;
    static uint64_t static_aligned_var __attribute__((aligned(128)));
    static al256int_t static_typed_aligned_var;

    printf("global variables:\n");
    print_align_results(&global_simple_var, &global_aligned_var, &global_typed_aligned_var);

    printf("stack variables:\n");
    print_align_results(&simple_var, &aligned_var, &typed_aligned_var);

    printf("static variables:\n");
    print_align_results(&static_simple_var, &static_aligned_var, &static_typed_aligned_var);
    return 0;
}
