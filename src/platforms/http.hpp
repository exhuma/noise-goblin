#pragma once
#include <string>
#include <vector>

/// @brief The HTTP subsystem
/// @details Interface for HTTP platform functionality.
///
/// Provides an abstract interface for HTTP operations.
struct IHttp {
    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~IHttp() = default;

    /// @brief Retrieve a list of resource names from a given URL.
    /// @details This assumes that the document retrieved from the URL conforms
    /// to the expected format for resource names. An example minimal document
    /// looks like this:
    ///
    ///     [
    ///         {"resource_str": "sound1"},
    ///         {"resource_str": "sound2"},
    ///         {"resource_str": "sound3"}
    ///     ]
    ///
    /// It only retrieves the names of the resources to save memory.
    ///
    /// @param url The URL to fetch the list of resource names from.
    /// @return A vector of strings containing the fetched resource names.
    virtual auto getResourceNames(std::string url)
        -> std::vector<std::string> = 0;
};
