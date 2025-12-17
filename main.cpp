#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

// 明るさを文字に変換するテーブル (暗い -> 明るい)
const std::string ASCII_CHARS = "@#$*!;=~-,. ";

int main() {
    // GIFファイルを開く
    // ★ポイント: ここを "test.gif" ではなく 0 にすると、Webカメラが起動して「鏡」になります
    cv::VideoCapture cap("nyancat.gif"); 

    if (!cap.isOpened()) {
        std::cerr << "GIFファイルが見つかりません！(test.gifを置いてね)" << std::endl;
        return -1;
    }

    // コンソール画面をクリア
    std::cout << "\033[2J";

    cv::Mat frame, gray, resized;
    
    // 表示サイズ（コンソールの文字数に合わせて調整）
    const int width = 160;
    const int height = 60; // 文字は縦長なので、幅より少なめにすると比率が合います

    while (true) {
        // 1フレーム読み込み
        cap >> frame;
        
        // GIFが最後まで行ったらループ再生するために巻き戻す
        if (frame.empty()) {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue;
        }

        // 1. カラーをグレースケール（白黒）に変換
        //cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // 2. コンソールに収まるサイズに縮小
        cv::resize(frame, resized, cv::Size(width, height));
        cv::convertScaleAbs(resized, resized, 1.0, 3.0);

        // 3. 描画用バッファを作る
        std::string buffer;
        buffer += "\033[H"; // カーソルを左上に戻す（画面クリアより高速）

        // ピクセルを文字にマッピング
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // 画素値 (0-255) を取得
                cv::Vec3b color = resized.at<cv::Vec3b>(y, x);
                int pixel = static_cast<int>(0.299 * color[2] + 0.587 * color[1] + 0.114 * color[0]); // グレースケール変換

                int r = color[2];
                int g = color[1];
                int b = color[0];

                // ANSIエスケープコードでカラー指定
                buffer += "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
                
                // ピクセル値を文字にマッピング
                int charIndex = (pixel * (ASCII_CHARS.size() - 1)) / 255;
                buffer += ASCII_CHARS[charIndex];
            }
            buffer += "\n";
        }

        // 一気に表示
        std::cout << buffer << std::flush;

        // 再生速度調整（GIFのFPSに合わせるのが理想ですが、簡易的に30ms待機）
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}