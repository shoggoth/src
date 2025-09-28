// The Swift Programming Language
// https://docs.swift.org/swift-book

import ArgumentParser
import PathKit

@main
struct filewatcher : ParsableCommand {
    @Option(help: "The path to the file to watch")
    var path: Path

    static func not_main() {
        print("Hello, world!")
    }

    mutating func run() throws {
        print("Run")
    }

    func validate() throws {
        guard path.isFile else {
            throw ValidationError("(path) is not a file")
        }

        guard path.exists else {
            throw ValidationError("(path) does not exist")
        }
    }
}

extension Path: @retroactive ExpressibleByArgument {
    public init?(argument: String) {
        self.init(argument)
    }
}
