from draw import draw_text, draw_pixbuf
from utils import get_text_size, cairo_disable_antialias
from color import color_hex_to_cairo
import gtk

class SelectButton(gtk.Button):        
    def __init__(self,
                 subject_index,
                 text,
                 ali_padding=0,
                 font_size=10,
                 bg_color="#63b8f1",):
        gtk.Button.__init__(self)
        # init values.
        self.text = text
        self.font_size=font_size 
        self.ali_padding = ali_padding
        self.bg_color = bg_color
        self.draw_check = False
        self.selected = False
        width, height = get_text_size(self.text, font_size)
        self.set_size_request(width, height)
        # init events.
        self.add_events(gtk.gdk.ALL_EVENTS_MASK)
        self.connect("button-press-event", self.select_button_button_press_event)
        self.connect("button-release-event", self.select_button_button_release_event)
        self.connect("expose-event", self.select_button_expose_event)        
        self.connect("clicked", self.button_clicked_event, subject_index)

    def button_clicked_event(self, widget, subject_index):
        if self.selected == False:
            self.selected = True
            self.queue_draw()
            print subject_index

    def select_button_button_press_event(self, widget, event):
        widget.grab_add()

    def select_button_button_release_event(self, widget, event):
        widget.grab_remove()
        
    def select_button_expose_event(self, widget, event):
        cr = widget.window.cairo_create()
        rect = widget.allocation
        # 
        # get font width/height.
        font_w, font_h = get_text_size(self.text, text_size=self.font_size)
        # draw text.
        x_padding = rect.x + rect.width - font_w - self.ali_padding
        x_padding = max(20, x_padding)

        if self.selected:
            # draw rectangle.
            with cairo_disable_antialias(cr):
                cr.set_source_rgb(*color_hex_to_cairo(self.bg_color))
                cr.rectangle(rect.x, 
                            rect.y, 
                            rect.width, 
                            rect.height)
                cr.fill()
        
            draw_text(cr, self.text,
                    x_padding,
                    rect.y + rect.height/2 - font_h/2,
                    text_size=self.font_size, 
                    text_color="#ffffff")        
        else:
            draw_text(cr, self.text,
                    x_padding,
                    rect.y + rect.height/2 - font_h/2,
                    text_size=self.font_size, 
                    text_color="#000000")        
        # set size.
        if font_h > rect.height:
            widget.set_size_request(rect.width, font_h)
        return True

class ImageButton(gtk.Button):        
    def __init__(self, image_file):
        gtk.Button.__init__(self)
        # init values.
        self.image_pixbuf = gtk.gdk.pixbuf_new_from_file(image_file)
        self.draw_check = False
        self.set_size_request(self.image_pixbuf.get_width(), self.image_pixbuf.get_height())
        # init events.
        self.add_events(gtk.gdk.ALL_EVENTS_MASK)
        self.connect("expose-event", self.image_button_expose_event)        

    def image_button_expose_event(self, widget, event):
        cr = widget.window.cairo_create()
        rect = widget.allocation

        draw_pixbuf(cr, self.image_pixbuf, rect.x, rect.y)
        return True
