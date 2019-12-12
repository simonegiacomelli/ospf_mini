function millis() {
    return Date.now();
}

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


class Rate {
    constructor(windowSecs) {
        this.windowSecs = windowSecs;
        this.counter = [];
        this.spinCount = 0;
        this.trackedSecond = -1;
        this.reset();
    }

    reset() {
        this.counter = [];
        this.counter.push(0);
        this.trackedSecond = this.currentSecond();
        return this;
    }

    spin() {
        this.spinCount++;
        this.update();
        this.counter[0]++;
    }

    update() {
        let currentSecond = this.currentSecond();

        let secs = currentSecond - this.trackedSecond;
        while (secs > 0) {
            this.counter.splice(0, 0, 0);
            secs--;
        }

        let exceedSize = this.counter.length - this.windowSecs;
        while (exceedSize > 0) {
            this.counter.pop();
            exceedSize--;
        }
        this.trackedSecond = currentSecond;

    }

    currentSecond() {
        let current = millis();
        let cs = Math.trunc(current / 1000);
        return cs;
    }

    rate() {
        this.update();
        if (this.counter.length === 0) return 0.0;
        let result = 0;
        for (let i = 0; i < this.counter.length; i++) {
            result += this.counter[i];
        }
        return result / this.counter.length;
    }


}