{
  "targets": [
    {
      "target_name": "compression",
      "sources": [ "compression.cpp", "TextWrapper.cpp", "TextWrapper.h", "src/TextComponent.h", "src/Decorator.h", "src/HuffmanEncoder.h", "src/LZWEncoder.h", "src/MTFEncoder.h", "src/PlainText.h",
                   "src/RLEEncoder.h", "src/BWTransform.h", "src/Encoding.h", "src/TextComponent.cpp", "src/Decorator.cpp", "src/HuffmanEncoder.cpp",
                   "src/LZWEncoder.cpp", "src/MTFEncoder.cpp", "src/PlainText.cpp", "src/RLEEncoder.cpp", "src/BWTransform.cpp", "src/Encoding.cpp"],
      "cflags": [
        "-std=c++11"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
