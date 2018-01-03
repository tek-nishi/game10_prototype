#pragma once

//
// 表示関連
//


// パネル１枚表示
void drawPanel(int number, glm::ivec2 pos, u_int rotation, const Texture& image) {
  int tx = (number % 8) * 128;
  int ty = (number / 8) * 128;

  const float r_tbl[] = {
    0.0f,
    M_PI * 0.5f,
    M_PI,
    M_PI * 1.5f 
  };

  drawTextureBox(pos.x, pos.y,
                 128, 128,
                 tx, ty,
                 128, 128,
                 image, Color(1, 1, 1),
                 r_tbl[rotation], Vec2f(1, 1),
                 Vec2f(64, 64));
}

// Fieldのパネルをすべて表示
void drawFieldPanels(const std::vector<PanelStatus>& panels, const Texture& image) {
  for (const auto& p : panels) {
    drawPanel(p.number, p.position * 128, p.rotation, image);
  }
}

// Fieldの置ける場所をすべて表示
void drawFieldBlank(const std::vector<glm::ivec2>& blank) {
  for (const auto& p : blank) {
    drawFillBox(p.x * 128 - 64, p.y * 128 - 64,
                128, 128,
                Color(0.3, 0.3, 0.3));
  }
}



// Grid表示
void drawFieldGrid() {
  for (int x = 0; x < 8; ++x) {
    drawLine(x * 128 - 512 - 64, -512,
             x * 128 - 512 - 64,  512,
             1,
             Color(0.5, 0.5, 0.5));
  }

  for (int y = 0; y < 8; ++y) {
    drawLine(-512, y * 128 - 512 - 64,
              512, y * 128 - 512 - 64,
             1,
             Color(0.5, 0.5, 0.5));
  }
}

