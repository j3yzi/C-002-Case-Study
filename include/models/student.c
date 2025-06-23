#include "../models/student.h"
#include <string.h>

/**
 * @brief Composes the student's full name based on their name parts.
 * @param name Pointer to the StudentName struct.
 * @return true if the name was composed successfully, false otherwise.
 */
bool composeStudentName(StudentName* name) {
    if (!name) {
        return false;
    }

    size_t fnLen = strlen(name->firstName);
    size_t mnLen = strlen(name->middleName);
    size_t lnLen = strlen(name->lastName);
    size_t totalLen = fnLen + mnLen + lnLen;

    // If total length is exactly studentNameLen - 1, use LastName+FirstName+MiddleName.
    if (totalLen == (studentNameLen - 1)) {
        snprintf(name->fullName, studentNameLen, "%s%s%s", name->lastName, name->firstName, name->middleName);
        return true;
    }

    // For all other cases, try LastName+FirstName.
    if ((lnLen + fnLen) < studentNameLen) {
        snprintf(name->fullName, studentNameLen, "%s%s", name->lastName, name->firstName);
        return true;
    }

    // If LastName+FirstName is too long, or the name is invalid, reject.
    return false;
} 