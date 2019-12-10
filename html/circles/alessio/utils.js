function bind(caller) {
    for (const key of Object.getOwnPropertyNames(caller.constructor.prototype)) {
        const value = caller[key];

        if (key !== 'constructor' && typeof value === 'function')
            caller[key] = value.bind(caller);
    }
}

class Hotkey {
    //element can be window for global hotkeys
    constructor(element) {
        this.element = element
        this.handlers = {};
        this.debug = false;
        bind(this);
        element.addEventListener("keydown", this.handler, false);
    }

    add(hotkey, callback) {
        if (this.debug)
            console.log(`Registering hotkey ${hotkey}`);
        this.handlers[hotkey.toUpperCase()] = callback;
        return this;
    }

    handler(ke) {
        if (!ke instanceof KeyboardEvent) return;
        let key = ke.key;
        if (key === 'Shift') return;
        if (key === 'Control') return;
        if (key === 'Alt') return;
        if (key === 'Meta') return;
        // val keh = KeyboardEventHelper(ke)

        // if (!keh.allowed) return
        let res = "";
        if (ke.ctrlKey) res += "CONTROL-";
        if (ke.shiftKey) res += "SHIFT-";
        if (ke.altKey) res += "ALT-";
        if (ke.metaKey) res += "META-";

        let hotkey1 = res + ke.code;
        let hotkey2 = res + ke.key;

        if (this.debug)
            console.log('hotkey recognize this two formats:', hotkey1, hotkey2);

        let h = this.handlers[hotkey1.toUpperCase()];
        if (h === undefined) h = this.handlers[hotkey2.toUpperCase()];
        if (h === undefined) return;
        h();
        ke.preventDefault();
        ke.stopPropagation();
    }

    enableDebug() {
        this.debug = true;
        return this;
    }
}
