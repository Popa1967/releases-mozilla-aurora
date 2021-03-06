"use strict";

const DEFAULT_AUDIO_SAMPLE_FILE = "hello.ogg";
const SPEECH_RECOGNITION_TEST_REQUEST_EVENT_TOPIC = "SpeechRecognitionTest:RequestEvent";
const SPEECH_RECOGNITION_TEST_END_TOPIC = "SpeechRecognitionTest:End";

var errorCodes = {
  NO_SPEECH : 0,
  ABORTED : 1,
  AUDIO_CAPTURE : 2,
  NETWORK : 3,
  NOT_ALLOWED : 4,
  SERVICE_NOT_ALLOWED : 5,
  BAD_GRAMMAR : 6,
  LANGUAGE_NOT_SUPPORTED : 7
};

netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
Components.utils.import("resource://gre/modules/Services.jsm");
SpecialPowers.setBoolPref("media.webspeech.recognition.enable", true);
SpecialPowers.setBoolPref("media.webspeech.test.enable", true);

function EventManager(sr) {
  var self = this;
  var nEventsExpected = 0;
  self.eventsReceived = [];

  var allEvents = [
    "audiostart",
    "soundstart",
    "speechstart",
    "speechend",
    "soundend",
    "audioend",
    "result",
    "nomatch",
    "error",
    "start",
    "end"
  ];

  var eventDependencies = {
    "speechend": "speechstart",
    "soundent": "soundstart",
    "audioend": "audiostart"
  };

  // AUDIO_DATA events are asynchronous,
  // so we queue events requested while they are being
  // issued to make them seem synchronous
  var isSendingAudioData = false;
  var queuedEventRequests = [];

  // register default handlers
  for (var i = 0; i < allEvents.length; i++) {
    (function (eventName) {
      sr["on" + eventName] = function (evt) {
        ok(false, "unexpected event: " + eventName);
        if (self.done) self.done();
      };
    })(allEvents[i]);
  }

  self.expect = function EventManager_expect(eventName, cb) {
    nEventsExpected++;

    sr["on" + eventName] = function(evt) {
      self.eventsReceived.push(eventName);
      ok(true, "received event " + eventName);

      var dep = eventDependencies[eventName];
      if (dep) {
        ok(self.eventsReceived.indexOf(dep) >= 0,
           eventName + " must come after " + dep);
      }

      cb && cb(evt, sr);
      if (self.done && nEventsExpected === self.eventsReceived.length) {
        self.done();
      }
    }
  }

  self.requestFSMEvent = function EventManager_requestFSMEvent(eventName) {
    if (isSendingAudioData) {
      info("Queuing event " + eventName + " until we're done sending audio data");
      queuedEventRequests.push(eventName);
      return;
    }

    var subject = null;

    if (eventName === "EVENT_AUDIO_DATA") {
      isSendingAudioData = true;
      var audioTag = document.createElement("audio");
      audioTag.src = self.audioSampleFile;

      subject = audioTag.mozCaptureStreamUntilEnded();
      audioTag.addEventListener("ended", function() {
        info("Sample stream ended, requesting queued events");
        isSendingAudioData = false;
        while (queuedEventRequests.length) {
          self.requestFSMEvent(queuedEventRequests.shift());
        }
      });

      audioTag.play();
    }

    info("requesting " + eventName);
    Services.obs.notifyObservers(subject,
                                 SPEECH_RECOGNITION_TEST_REQUEST_EVENT_TOPIC,
                                 eventName);
  }

  self.requestTestEnd = function EventManager_requestTestEnd() {
    Services.obs.notifyObservers(null, SPEECH_RECOGNITION_TEST_END_TOPIC, null);
  }
}

function resetPrefs() {
  SpecialPowers.setBoolPref("media.webspeech.test.fake_fsm_events", false);
  SpecialPowers.setBoolPref("media.webspeech.test.fake_recognition_service", false);
}

function buildResultCallback(transcript) {
  return (function(evt) {
    is(evt.results[0][0].transcript, transcript, "expect correct transcript");
  });
}

function buildErrorCallback(errcode) {
  return (function(err) {
    is(err.error, errcode, "expect correct error code");
  });
}

function performTest(eventsToRequest, expectedEvents, doneFunc, audioSampleFile) {
  var sr = new SpeechRecognition();
  var em = new EventManager(sr);

  for (var eventName in expectedEvents) {
    var cb = expectedEvents[eventName];
    em.expect(eventName, cb);
  }

  em.done = function() {
    em.requestTestEnd();
    resetPrefs();
    doneFunc();
  }

  if (!audioSampleFile) {
    audioSampleFile = DEFAULT_AUDIO_SAMPLE_FILE;
  }

  em.audioSampleFile = audioSampleFile;

  for (var i = 0; i < eventsToRequest.length; i++) {
    em.requestFSMEvent(eventsToRequest[i]);
  }
}
