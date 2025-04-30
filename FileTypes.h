#ifndef FILETYPES_H
#define FILETYPES_H

#include <QString>

/**
 * @brief Abstract base class representing a file in a project.
 *
 * This class is intended to be subclassed for different types of project files,
 * such as class files, namespace files, or free function files.
 */
class ProjectFile {
public:
    virtual ~ProjectFile() = default;
};

#endif // FILETYPES_H
