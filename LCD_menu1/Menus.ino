
int get_nextMenu(int current_menu) {
  if (parameters_set[0] == 0) {
    //mode IV
    for (int i = current_menu; i < numOfScreens; i++ ) {
      if ( IV_menus[i] == true ) {
        return i;
      }
    }
  }
  else {
    //mode RPM
    for (int i = current_menu; i < numOfScreens; i++ ) {
      if ( RPM_menus[i] == true ) {
        return i;
      }
    }
  }
  return 0;
}
