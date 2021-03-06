#include <stdio.h>

#include "smollisp/VM.h"

int main(void) {
  smollisp_VM vm;
  smollisp_VM_new(&vm);
  smollisp_Result result = SMOLLISP_RESULT_OK;

  smollisp_VM_begin_frame(&vm);
  smollisp_Frame *frame1 = smollisp_VM_get_top_frame(&vm);

  smollisp_Value i_8;
  smollisp_Value_new_int32(&i_8, 8);
  smollisp_Frame_push(frame1, i_8);

  // frame 2
  smollisp_VM_begin_frame(&vm);
  smollisp_Frame *frame2 = smollisp_VM_get_top_frame(&vm);

  smollisp_Value i_16;
  smollisp_Value i_32;

  smollisp_Value_new_int32(&i_16, 16);
  smollisp_Value_new_int32(&i_32, 32);

  smollisp_Frame_push(frame2, i_16); // i_16 moved
  smollisp_Frame_push(frame2, i_32); // i_32 moved

  result = smollisp_Frame_do_op(frame2, SMOLLISP_OP_ADD);
  SMOLLISP_ASSERT(result == SMOLLISP_RESULT_OK);

  result = smollisp_Frame_return(frame2, 1, frame1);
  SMOLLISP_ASSERT(result == SMOLLISP_RESULT_OK);
  smollisp_VM_end_frame(&vm);

  result = smollisp_Frame_do_op(frame1, SMOLLISP_OP_ADD);
  SMOLLISP_ASSERT(result == SMOLLISP_RESULT_OK);

  smollisp_Value string_1;
  smollisp_Value_new_string(&string_1, "Hello");

  smollisp_Value string_2;
  smollisp_Value_new_string(&string_2, ", world!");

  smollisp_Frame_push(frame1, string_1);         // string_1 moved
  smollisp_Frame_push(frame1, string_2);         // string_2 moved
  smollisp_Frame_do_op(frame1, SMOLLISP_OP_ADD); // concat strings

  smollisp_Value v;
  smollisp_Value_new_int32(&v, 4);
  smollisp_Context_define_var(frame1->ctx, ", world!Hello", v); // v moved

  result = smollisp_Frame_do_op(frame1, SMOLLISP_OP_LOOKUP);
  SMOLLISP_ASSERT(result == SMOLLISP_RESULT_OK);
  result = smollisp_Frame_do_op(frame1, SMOLLISP_OP_ADD);
  SMOLLISP_ASSERT(result == SMOLLISP_RESULT_OK);

  printf("%d\n", frame1->stack[0].int32);

  smollisp_VM_end_frame(&vm);

  return 0;
}
