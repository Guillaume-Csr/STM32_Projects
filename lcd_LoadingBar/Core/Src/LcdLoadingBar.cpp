#include "LcdLoadingBar.h"

void displayBar(int percent, int line) {

  /* Déplace le curseur sur la ligne */
  lcd_put_cur(0, line);

  /* Map la plage (0 ~ 100) vers la plage (0 ~ (LCD_NB_COLUMNS - 4) * 2 * 4 - 2 * 4) */
  char nb_columns = MapArray(percent, 0, 100, 0, (LCD_NB_COLUMNS - 4) * 2 * 4 - 2 * 4);
  // Chaque caractére affiche 2 barres verticales de 4 pixels de haut, mais le premier et dernier caractére n'en affiche qu'une.

  /* Dessine chaque caractére de la ligne */
  for (int i = 0; i < LCD_NB_COLUMNS - 4; ++i) {

    if (i == 0) { // Premiére case

      /* Affiche le char de début en fonction du nombre de colonnes */
      if (nb_columns > 4) {
    	lcd_send_data(0); // Char début 4 / 4
        nb_columns -= 4;

      } else if (nb_columns == 4) {
    	lcd_send_data(0); // Char début 4 / 4
        nb_columns = 0;

      } else {
        switch_progressbar_bank(0);
        lcd_put_cur(i, line);
        lcd_send_data(nb_columns + 4); // Char début N / 4
        nb_columns = 0;
      }

    } else if (i == LCD_NB_COLUMNS - 5) { // Derniére case

      /* Affiche le char de fin en fonction du nombre de colonnes */
      if (nb_columns > 0) {
        switch_progressbar_bank(3);
        lcd_put_cur(i, line);
        lcd_send_data(nb_columns + 3); // Char fin N / 4

      } else {
    	  lcd_send_data(3); // Char fin 0 / 4
      }

    } else { // Autres cases

      /* Affiche le char adéquat en fonction du nombre de colonnes */
      if (nb_columns == 0) {
    	  lcd_send_data(1); // Char div 0 / 8

      } else if (nb_columns >= 8) {
    	  lcd_send_data(2); // Char div 8 / 8
        nb_columns -= 8;

      } else if (nb_columns >= 4 && nb_columns < 8) {
        switch_progressbar_bank(2);
        lcd_put_cur(i, line);
        lcd_send_data(nb_columns); // Char div N / 8
        nb_columns = 0;

      } else if (nb_columns < 4) {
        switch_progressbar_bank(1);
        lcd_put_cur(i, line);
        lcd_send_data(nb_columns + 3); // Char div N / 8
        nb_columns = 0;
      }
    }
  }

  /* Affiche le pourcentage */
  char tmp[5];
  fprintf(tmp, "%3d%%", percent);
  lcd_send_string(tmp);
}

void switch_progressbar_bank(int bank)
{
	switch (bank)
	{
	case 0:
		lcd_send_cmd(START_DIV_0_OF_4);
		lcd_send_cmd(START_DIV_1_OF_4);
		lcd_send_cmd(START_DIV_2_OF_4);
		lcd_send_cmd(START_DIV_3_OF_4);
		break;
	case 1:
		lcd_send_cmd(DIV_1_OF_8);
		lcd_send_cmd(DIV_2_OF_8);
		lcd_send_cmd(DIV_3_OF_8);
		lcd_send_cmd(DIV_4_OF_8);
	case 2:
		lcd_send_cmd(4, DIV_4_OF_8);
		lcd_send_cmd(5, DIV_5_OF_8);
		lcd_send_cmd(6, DIV_6_OF_8);
		lcd_send_cmd(7, DIV_7_OF_8);
		break;
	case 3:
		lcd_send_cmd(4, END_DIV_1_OF_4);
		lcd_send_cmd(5, END_DIV_2_OF_4);
		lcd_send_cmd(6, END_DIV_3_OF_4);
		lcd_send_cmd(7, END_DIV_4_OF_4);
		break;

	}
}

void MapArray(void ** src, void * dest, void * (f)(void *), size_t n, size_t sizeofT)
{
    for(unsigned int i = 0; i < n; i++) {
        void* temp = f(src[n]);
        memcpy(dest, temp, sizeofT);
        dest = (char*)dest + sizeofT;
    }
}
