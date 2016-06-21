
function open_folder(){

}
exports.chooseFile = function(name) {
    var chooser = global.$(name);
    // chooser.change(function(evt) {
    //     callback($(this).val());
    // });

    chooser.trigger('click');
};
exports.open_folder = open_folder;
