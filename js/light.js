
  // Get all values and prepare them:
  function current_values() {

    var rtn = [];
    var colors = $('#color-picker').spectrum('get');
    var flicker_range = $("#flicker-range").val();
    var flicker_delay = $("#flicker-delay").val();

    rtn.push(Math.floor(colors['_r']*4));
    rtn.push(Math.floor(colors['_g']*4));
    rtn.push(Math.floor(colors['_b']*4));
    rtn.push(Math.floor(flicker_range));
    rtn.push(Math.floor(flicker_delay));

    return rtn;

  }

  function set_ui_values() {

    $.ajax({
      url: 'set.php?current',
      method: 'get',
      success: function(data){

        // Parse string into it's individual values.
        var vals = JSON.parse(data);

        rgb = []; // Initialize array to hold color values.

         // Set values back to a 4th of value on device:
        rgb.push(Math.floor(vals[0]/4)); // Red.
        rgb.push(Math.floor(vals[1]/4)); // Green.
        rgb.push(Math.floor(vals[2]/4)); // Blue.
        var flicker_range = Math.floor(vals[3]); // Flicker Range.
        var flicker_delay = Math.floor(vals[4]); // Flicker Delay.

        color_values = 'rgb('+rgb.join(',')+')';

        $('#color-picker').spectrum('set', color_values );
        $('#light').css('background-color', color_values);
        $('#flicker-range').val(flicker_range);
        $('#flicker-delay').val(flicker_delay);

      }
    });
  }
    $(document).ready(function(){

      set_ui_values();
      current_values();
      //setInterval(set_ui_values, 5000);

      picker = $("#color-picker").spectrum({
          flat: true,
          showButtons: false,
          preferredFormat: "rgb",
          showInput: true
      }).on("dragstop.spectrum", function(e, color) {
          $.ajax({
            url: 'set.php?all=1&values='+current_values(),
            method: 'get'
          }).done(set_ui_values());
      });

      $('.range').on('input',function(){
        var element = $(this);
        var name = element.data('name');
        var value = element.find('input').val();
        var label = element.find('.range-value');
        label.html(value);
      });

      $('.range').change(function(){
        $.ajax({
          url: 'set.php?all=1&values='+current_values(),
          method: 'get'
        }).done(set_ui_values());
      });

    });
