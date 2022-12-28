#include <gtkmm.h>
#include <gtkmm/progressbar.h>

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.gtkmm.examples.base");

  Gtk::Window window;
  window.set_default_size(200, 200);
  Gtk::ProgressBar progBar;

  
  progBar.set_fraction(.5);
  window.add(progBar); 
  progBar.set_halign(Gtk::ALIGN_CENTER);
  progBar.set_valign(Gtk::ALIGN_CENTER);
  progBar.set_text("some text");
  progBar.set_show_text(true);
  app->run(window);
  

  

  return 0;
}
