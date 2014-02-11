//
// Class: SomeViewClass
// Some view functions
//

SomeViewClass = Class.create( 
{
});


SomeViewClass.currentSome = null;
SomeViewClass.currentSomeFile = null;
SomeViewClass.percentUpload = 0;


//
// static public methods
// @scope SomeViewClass
//
Object.extend(SomeViewClass, {

    change: function(event)
    {
        debugWindow(arguments.callee.name)

        var saveButton = document.getElementById('someEditorSaveButton').object;
        var textField = document.getElementById('someEditorNameTextField');
        if (textField != null && textField.value != '') {
            saveButton.setEnabled(true);
        } else {
            saveButton.setEnabled(false);
        }
    },


    edit: function()
    {
        debugWindow(arguments.callee.name);
        
        SomeViewClass.currentSomeFile = null;
        var ds = dashcode.getDataSource('some');
        SomeViewClass.currentSome = ds.__content[0];
        
        var expertEditorNameTextField = document.getElementById('someEditorNameTextField');
        
        someEditorImageHolderField.value = '';
        someEditorNameTextField.value = SomeViewClass.currentSome.name;
        
        if (SomeViewClass.currentSome.someUrl != '' && SomeViewClass.currentSome.someUrl != null && SomeViewClass.currentSome.someUrl != 'None')
        {
            if (typeof(FileReader) == 'undefined')
            {
                someEditorImageField.childNodes[1].src = SomeViewClass.currentSome.someUrl;
                hideNode('someEditorImageHolderField');
            }
            else
            {
                someEditorImageField.childNodes[1].src = SomeViewClass.currentSome.someUrl;
            }
        }
    },
    
    updatePercent: function(e)
    {
        debugWindow(arguments.callee.name)

        var done = e.position || e.loaded, total = e.totalSize || e.total;
        var percent = (Math.floor(done/total*1000)/10);
        if (isFinite(percent))
        {
            SomeViewClass.percentUpload = percent;
        }
        else
        {
            SomeViewClass.percentUpload = 100;
        }
        debugWindow('xhr progress: ' + SomeViewClass.percentUpload + '%');

        var someEditorUploadPercent = document.getElementById('someEditorUploadPercent');
        someEditorUploadPercent.innerHTML = SomeViewClass.percentUpload + '%';
        
        var someEditorGauge = document.getElementById("someEditorGauge");
        someEditorGauge.object.setValue(SomeViewClass.percentUpload);


    },

    save: function(event)
    {
        debugWindow(arguments.callee.name)
        var someEditorNameTextField = document.getElementById('someEditorNameTextField');

        showNodeLoading();
        
        var finishBlock = function()
        {
            // name
            SomeViewClass.currentSome.name = someEditorNameTextField.value;
            SomeViewClass.currentSome.parseObject.set('name', someEditorNameTextField.value);

            SomeViewClass.currentSome.parseObject.save();
            
            hideNodeLoading();

            goToMenu();
        }

        
        if (SomeViewClass.currentSomeFile != '' && SomeViewClass.currentSomeFile != null)
        {
        
            var _urlSome = "http://localhost/~thesport/";
            if (_development)
            {
                _urlSome += "devel/";
            }
            else
            {
                _urlSome += "some/";
            }

            var name = SomeViewClass.currentSomeFile.fileName || SomeViewClass.currentSomeFile.name;
            debugWindow('saving: filename '+ name);
            debugWindow('saving: type ' + SomeViewClass.currentSomeFile.type);
            
            SomeViewClass.percentUpload = 0;
            showNode('someEditorUploadPercent');
            showNode('someEditorGauge');
            
            // The feed to fetch
            var xmlRequest = new XMLHttpRequest();
            xmlRequest.addEventListener('progress', function(e) {
                SomeViewClass.updatePercent(e);
                }, false);
            if ( xmlRequest.upload ) 
            {
                xmlRequest.upload.onprogress = function(e) {
                    SomeViewClass.updatePercent(e);
                };
            }

            xmlRequest.onreadystatechange = function(e) {
                if ( 4 == this.readyState ) {
                    debugWindow(['xhr upload complete', e]);
                    hideNode('someEditorUploadPercent');
                    hideNode('someEditorGauge');

                    if (SomeViewClass.percentUpload == 100)
                    {
                        SomeViewClass.currentSome.someUrl = _urlSome + name;
                        SomeViewClass.currentSome.parseObject.set('someUrl', _urlSome + name);

                        finishBlock();            
                    }
                    else
                    {
                        showErrorView('Failed to save some file', {
                            close: function()
                            {
                                hideNodeLoading();
                                goToMenu();
                            }
                        });

                    }
                }
            };
            var url = _urlSome + "input.php?name="+name;
            debugWindow("url to upload: " + url);
            xmlRequest.open('post', url, true);
            xmlRequest.setRequestHeader("Content-Type", "multipart/form-data");
            xmlRequest.send(SomeViewClass.currentSomeFile);

        }
        else
        {
            finishBlock();            
        }
    },



    ondragendHolderSome: function (event)
    {
        debugWindow(arguments.callee.name);
        var someEditorImageHolder = document.getElementById('someEditorImageHolder');
        someEditorImageHolder.className = ''; 

        showNode('someEditorImageMessageText');

        return false; 
    },


    ondragoverHolderSome: function(event)
    {
        debugWindow(arguments.callee.name);
        var someEditorImageHolder = document.getElementById('someEditorImageHolder');
        someEditorImageHolder.className = 'hover'; 

        hideNode('someEditorImageMessageText');
        
        return false; 
    },


    ondropHolderSome: function(event)
    {
        debugWindow(arguments.callee.name);
        var someEditorImageHolder = document.getElementById('someEditorImageHolder');
        someEditorImageHolder.className = '';
        event.preventDefault();

        SomeViewClass.currentSomeFile = event.dataTransfer.files[0];
        var name = SomeViewClass.currentSomeFile.fileName || SomeViewClass.currentSomeFile.name;
        debugWindow('File is: ' + name);

        var someEditorImageHolderField = document.getElementById('someEditorImageHolderField');
        
        if (typeof(FileReader) == 'undefined')
        {
            debugWindow('FileReader not supported');
        }
        else
        {
            debugWindow('FileReader YES');
            var reader = new FileReader();
            reader.onload = function (event) {
                var someEditorImageField = document.getElementById('someEditorImageField');
                someEditorImageField.childNodes[1].src = 'url(' + event.target.result + ')';

            };
            reader.readAsDataURL(SomeViewClass.currentSomeFile);
            
        }
        return false;

    },


    onchangeSomeImageHolderField: function(event)
    {
        debugWindow(arguments.callee.name);
        var someEditorImageHolderField = document.getElementById('someEditorImageHolderField');
        var someEditorImageHolder = document.getElementById('someEditorImageHolder');
        someEditorImageHolder.className = '';
        event.preventDefault();

        SomeViewClass.currentSomeFile = someEditorImageHolderField.files[0];
        var name = SomeViewClass.currentSomeFile.fileName || SomeViewClass.currentSomeFile.name;
        debugWindow('File is: ' + name);

        var someEditorImageHolderField = document.getElementById('someEditorImageHolderField');
        
        if (typeof(FileReader) == 'undefined')
        {
            debugWindow('FileReader not supported');
        }
        else
        {
            debugWindow('FileReader YES');
            var reader = new FileReader();
            reader.onload = function (event) {
                console.log(event.target);
                someEditorImageField.childNodes[1].src = 'url(' + event.target.result + ')';
           };
            reader.readAsDataURL(SomeViewClass.currentSomeFile);

        }
        return false;

    },


    ondragendSomeEditorImageField: function(event)
    {
        debugWindow(arguments.callee.name);
        var someEditorImageField = document.getElementById('someEditorImageField');
        someEditorImageField.className = ''; 

        showNode('someEditorImageMessageText');

        return false; 
    },


    ondragoverSomeEditorImageField: function(event)
    {
        debugWindow(arguments.callee.name);
        var someEditorImageField = document.getElementById('someEditorImageField');
        someEditorImageField.className = 'hover'; 

        hideNode('someEditorImageMessageText');
        
        return false; 
    },


    ondropSomeEditorImageField: function(event)
    {
        event.cancelBubble = true;
    },




});
