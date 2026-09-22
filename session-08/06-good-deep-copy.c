/* 06-good -- if a struct owns a pointer, copying it means copying the target.
 *
 * Write the copy function next to the struct. There is no operator=, no
 * __copy__, no deepcopy() -- if you do not write it, the language will
 * happily hand out shallow copies forever.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;   /* owned */
    int   id;
} Student;

static Student student_make(const char *name, int id)
{
    Student s = {.name = malloc(strlen(name) + 1), .id = id};
    if (s.name != NULL) {
        memcpy(s.name, name, strlen(name) + 1);
    }
    return s;
}

/* Deep copy: new struct, new string. Caller frees both with student_free. */
static Student student_copy(const Student *src)
{
    return student_make(src->name, src->id);
}

static void student_free(Student *s)
{
    free(s->name);
    s->name = NULL;   /* free, then NULL -- Session 7 */
}

int main(void)
{
    Student a = student_make("Ada", 1815);
    if (a.name == NULL) {
        perror("malloc");
        return 1;
    }

    Student b = student_copy(&a);
    if (b.name == NULL) {
        student_free(&a);
        perror("malloc");
        return 1;
    }
    b.id = 9999;

    printf("a.name = %p  b.name = %p   <-- different blocks\n",
           (void *)a.name, (void *)b.name);

    b.name[0] = 'X';
    printf("a = %s (%d)\n", a.name, a.id);
    printf("b = %s (%d)\n", b.name, b.id);

    student_free(&b);
    student_free(&a);
    return 0;
}
