/* 05 -- a struct on the heap, and the create/destroy pair.
 *
 * Session 7 said: every malloc needs a matching free, and C has no syntax
 * for ownership. Structs give you a place to put that contract. The standard
 * shape is a pair of functions:
 *
 *     Thing *thing_create(...);   allocates, returns NULL on failure
 *     void   thing_destroy(Thing *t);   frees everything, tolerates NULL
 *
 * One malloc for the struct, one for each pointer inside it -- and
 * thing_destroy undoes them in the opposite order.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char  *name;    /* owned: allocated here, freed in student_destroy */
    int    id;
    double gpa;
} Student;

/* Returns a Student the CALLER must pass to student_destroy(). NULL on failure. */
static Student *student_create(const char *name, int id, double gpa)
{
    Student *s = malloc(sizeof *s);
    if (s == NULL) {
        return NULL;
    }

    s->name = malloc(strlen(name) + 1);   /* + 1 for the '\0' */
    if (s->name == NULL) {
        free(s);                          /* no half-built objects */
        return NULL;
    }
    memcpy(s->name, name, strlen(name) + 1);

    s->id  = id;
    s->gpa = gpa;
    return s;
}

/* Safe to call with NULL, which makes error paths trivial. */
static void student_destroy(Student *s)
{
    if (s == NULL) {
        return;
    }
    free(s->name);    /* inside-out: the members first ... */
    free(s);          /* ... then the struct itself */
}

int main(void)
{
    Student *s = student_create("Grace Hopper", 1906, 4.0);
    if (s == NULL) {
        perror("student_create");
        return 1;
    }

    printf("%s (id %d), gpa %.2f\n", s->name, s->id, s->gpa);
    printf("sizeof *s = %zu bytes -- the name is NOT in there, only a pointer\n",
           sizeof *s);

    student_destroy(s);
    s = NULL;

    student_destroy(s);   /* no-op, by design */
    return 0;
}
