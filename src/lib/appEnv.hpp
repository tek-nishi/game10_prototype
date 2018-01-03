﻿#pragma once

//
// アプリの実行環境
//
//   アプリウィンドウを生成し、描画環境を整える役。
//   マウス・キー入力も担当しています
//
//   NOTICE:このクラスはコピー禁止
// 


#include "defines.hpp"
#include <set>
#include <vector>
#include "glfwWindow.hpp"
#include "vector.hpp"
#include "camera2D.hpp"
#include "graph.hpp"
#include "audio.hpp"
#include "gamePad.hpp"
#include "os.hpp"


// マウスボタンの省略名を定義
enum class Mouse {
  LEFT  = GLFW_MOUSE_BUTTON_LEFT,
  RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
};

// よく使うキーの別名を定義
enum Key {
  KEY_ESC           = GLFW_KEY_ESCAPE,
  KEY_ENTER         = GLFW_KEY_ENTER,
  KEY_TAB           = GLFW_KEY_TAB,
  KEY_RIGHT         = GLFW_KEY_RIGHT,
  KEY_LEFT          = GLFW_KEY_LEFT,
  KEY_DOWN          = GLFW_KEY_DOWN,
  KEY_UP            = GLFW_KEY_UP,
  KEY_LEFT_SHIFT    = GLFW_KEY_LEFT_SHIFT,
  KEY_RIGHT_SHIFT   = GLFW_KEY_RIGHT_SHIFT,
  KEY_LEFT_CONTROL  = GLFW_KEY_LEFT_CONTROL,
  KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
  KEY_LEFT_ALT      = GLFW_KEY_LEFT_ALT,
  KEY_RIGHT_ALT     = GLFW_KEY_RIGHT_ALT,
};

// 画面モード
enum class Screen {
  DEFAULT,          // ありのまま
  DYNAMIC,          // ウインドウサイズに合わせて内容を拡大縮小
  FULL,             // フルスクリーン
};


class AppEnv {
  // OS固有処理
  Os os_;
  
  // 描画ウィンドウ
  GlfwWindow window_;

  // 描画領域のサイズをWindowのサイズと連動する
  bool dynamic_window_size_;

  // 生成時のウィンドウサイズ
  Vec2f window_size_;
  
  // 現在のウィンドウサイズ
  Vec2f current_window_size_;

  Vec2i viewport_ofs_;
  Vec2i viewport_size_;
  
  // 2D座標変換用カメラ
  Camera2D camera_2d_;

  // 背景色
  Color bg_color_;
  
  // キー入力関連
  u_int pushed_key_;

  std::set<int> push_keys_;
  std::set<int> pull_keys_;
  std::set<int> press_keys_;

  // マウス関連
  Vec2f mouse_current_pos_;

  std::set<int> push_buttons_;
  std::set<int> pull_buttons_;
  std::set<int> press_buttons_;

  // Windowのフォーカス
  bool is_focus_;
  
  // GamePad
  std::vector<GamePad> gamepads_;
  
  // サウンド関連
  Audio audio_;
  
  
public:
  // width, height 生成時のサイズ
  // full_screen   true: フルスクリーン
  // dynamic_size  true: ウインドウサイズにあわせて画面を変更
  AppEnv(const int width, const int height,
         const Screen type = Screen::DEFAULT);

  // TIPS:このクラスはコピー禁止
  AppEnv(const AppEnv&) = delete;
  AppEnv& operator=(const AppEnv&) = delete;


  // アプリウインドウが開いてるならtrueを返す
  bool isOpen();

  // GLFWのハンドルを返却
  // TIPS:const版も定義
  // TIPS:*の右側のconstはポインタ値をconstにする
  GLFWwindow* const getGlfwHandle();
  const GLFWwindow* const getGlfwHandle() const;

  // アプリ更新処理開始
  void begin();

  // アプリ更新処理終了
  // 1. OpenGLの描画内容をウインドウに表示
  // 2. キーやマウスイベントのポーリング
  void end();
  
  // 入力(キー＆ボタン)の再初期化
  void flushInput();
  
  // ウィンドウサイズを返す
  const Vec2f& viewSize() const;

  // ウインドウの位置を変更
  // pos 新しい位置
  void windowPosition(const Vec2i& pos);

  // ウインドウの位置を取得
  Vec2i windowPosition();


  // 背景色の変更
  // color 色
  void bgColor(const Color& color);

  
  // 押されたキーを取得
  // 'A'とか'a'とか(押されてないときは0)
  // キーリピート付き
  u_int getPushedKey() const;

  
  // 当該キーが押されているならtrueを返す
  // key 'A'とか'7'とか
  // SOURCE:include/GLFW/glfw3.h 271〜396
  bool isKeyPressing(const int key) const;
  
  // 当該キーが押された瞬間trueを返す
  // key 'A'とか'7'とか
  // SOURCE:include/GLFW/glfw3.h 271〜396
  bool isKeyPushed(const int key) const;

  // 当該キーが離された瞬間trueを返す
  bool isKeyReleased(const int key) const;

  
  // マウスのカーソル位置を返す
  const Vec2f& mousePosition() const;

  // カーソル位置変更
  void mousePosition(const Vec2f& pos);
  
  // マウスカーソルのON/OFF
  void mouseCursor(const bool disp);
  
  // 当該ボタンが押されているならtrueを返す
  // button Mouse::LEFT
  //        Mouse::Right
  bool isButtonPressing(const Mouse button) const;
  
  // 当該ボタンが押された瞬間trueを返す
  // button Mouse::LEFT
  //        Mouse::RIGHT
  bool isButtonPushed(const Mouse button) const;

  // 当該ボタンが離された瞬間trueを返す
  // button Mouse::LEFT
  //        Mouse::RIGHT
  bool isButtonReleased(const Mouse button) const;

  // フォーカスの状態
  bool isFocus() const;
  
  // GamePadの接続数
  size_t numGamePad() const;

  // 指定番号のGamePadを取得
  // TIPS:const版も定義
  const GamePad& gamePad(const int index) const;
  GamePad& gamePad(const int index);

  
private:
  // キーが押された時に呼び出される
  static void createCharaInfo(GLFWwindow* window, const u_int chara);

  static void createKeyInfo(GLFWwindow* window,
                            const int key, const int scancode, const int action, const int mods);

  static void changeWindowSize(GLFWwindow* window, const int width, const int height);

  
  static void mouseButtonCallback(GLFWwindow* window,
                                  const int button, const int action, const int mods);

  static void mouseMoveCallback(GLFWwindow* window,
                                const double x_pos, const double y_pos);

  static void focusCallback(GLFWwindow* window, int focus);
  
  // 画面中央が(0, 0)の座標を計算
  static Vec2f screenPosition(const Vec2f& pos, const Vec2f& window, const Vec2f& viewport);

  // 画面中央が(0, 0)の座標→左上が(0, 0)の座標
  static Vec2f windowPosition(const Vec2f& pos, const Vec2f& window, const Vec2f& viewport);

  // 入力バッファを切り替える
  void switchInputBuffer();

  // 画面モード判定
  static bool isDynamic(const Screen type);
  static bool isFullscreen(const Screen type);
  
  // 動的Viewport(アスペクト比固定)
  void dynamicViewport(const int width, const int height);
  
};
