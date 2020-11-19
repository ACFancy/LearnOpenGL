import XCTest
@testable import OpenGLApp

final class OpenGLAppTests: XCTestCase {
    func testExample() {
        // This is an example of a functional test case.
        // Use XCTAssert and related functions to verify your tests produce the correct
        // results.
        XCTAssertEqual(OpenGLApp().text, "Hello, World!")
    }

    static var allTests = [
        ("testExample", testExample),
    ]
}
