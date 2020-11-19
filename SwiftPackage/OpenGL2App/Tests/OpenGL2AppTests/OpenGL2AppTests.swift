import XCTest
@testable import OpenGL2App

final class OpenGL2AppTests: XCTestCase {
    func testExample() {
        // This is an example of a functional test case.
        // Use XCTAssert and related functions to verify your tests produce the correct
        // results.
        XCTAssertEqual(OpenGL2App().text, "Hello, World!")
    }

    static var allTests = [
        ("testExample", testExample),
    ]
}
