//
//  ViewController.swift
//  Trifire
//
//  Created by Patrick Miller on 6/30/25.
//

import UIKit
import AudioToolbox
import AVFoundation


class ViewController: UIViewController {
    
    @IBOutlet weak var leftButton: UIButton!
    @IBOutlet weak var rightButton: UIButton!
    @IBOutlet weak var spinButton: UIButton!
    @IBOutlet weak var fireButton: UIButton!
    @IBOutlet weak var gamescreen: UIImageView!
    @IBOutlet weak var turnLabel: UILabel!
    @IBOutlet weak var scoreLabel: UILabel!
    @IBOutlet weak var hintButton: UIButton!
    
    // Probably a way to statically allocate these
    let width = 640
    let height = 480
    var state = UnsafeMutablePointer<Int>.allocate(capacity: 8)
    let arena = UnsafeMutablePointer<Int32>.allocate(capacity: 640*480);
    var coins : [Int] = []
    var trajectory : Int32 = -1
    var gameOver = false
    
    var audioPlayer: AVAudioPlayer?
    
    func boom() {
        print("try to boom")
        guard let url = Bundle.main.url(forResource: "boom", withExtension: "mp3") else {
            print("Sound file not found")
            return
        }
        print(url)
        
        do {
            print("update audioPlayer")
            audioPlayer = try AVAudioPlayer(contentsOf: url)
            print("set volume")
            audioPlayer?.volume = 1.0
            print("prepare")
            audioPlayer?.prepareToPlay()
            print("play")
            audioPlayer?.play()
        } catch {
            print("Error loading sound: \(error)")
        }
        print("done with boom")
    }
    
    func play(command: Character) {
        print("Play here", command)
        // If you play something, the hint goes away
        resetHint()
        
        if let scalar = command.unicodeScalars.first {
            print("command",command, scalar.value)
            
            let hit = playlong(Int8(scalar.value),state)
            print("play",hit)
            print("render",renderlong(arena, UInt32(width), UInt32(height), state))
            let turn = state[0]
            // We want to detect the explosion when we render, not when we play next turn
            if hit == 2 {
                boom()
                newCoin()
            }
            
            // Create pixel buffer (RGBA8888)
            let bytesPerPixel = 4
            let bytesPerRow = bytesPerPixel * width
            
            // Create CGContext
            let colorSpace = CGColorSpaceCreateDeviceRGB()
            let context = CGContext(
                data: arena,
                width: width,
                height: height,
                bitsPerComponent: 8,
                bytesPerRow: bytesPerRow,
                space: colorSpace,
                bitmapInfo: CGImageAlphaInfo.premultipliedLast.rawValue
            )
            
            // Create CGImage
            DispatchQueue.main.async {
                self.turnLabel.text = "\(turn)"
                self.scoreLabel.text = "\(self.coins.count+1)"
                if let cgImage = context?.makeImage() {
                    let image = UIImage(cgImage: cgImage)
                    self.gamescreen.image = image
                }
            }
        }
    }
    
    func resetHint() {
        DispatchQueue.main.async {
            self.hintButton.setTitle("Hint", for: .normal)
        }
    }
    
    func newCoin() {
        if coins.count == 0 {
            gameOver = true
            trajectory = -1
            state[2] = 0
            state[3] = 0
            state[4] = 0
            state[5] = 0
            state[6] = 0
            state[7] = 0
            DispatchQueue.main.async {
                self.scoreLabel.text = "0"
                self.hintButton.setTitle("WINNER", for: .normal)
            }
        } else {
            print("before",coins)
            let c = coins.popLast()
            print("after ",coins)
            trajectory = setcoin(Int32(c!), state) - 1
            print("Use coin",c,state[4],state[5])
            print("target x",trajectory % 18,"target spin", trajectory / 18 )
            state[6] = 0 // cannon ball done
            state[7] = 0 // cannon ball done
            print("new trajectory", trajectory, "Coins left", coins.count)
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Set up audio
        do {
            try AVAudioSession.sharedInstance().setCategory(.playback, mode: .default)
            try AVAudioSession.sharedInstance().setActive(true)
        } catch {
            print("Failed to set audio session: \(error)")
        }
        
        // We make sure to serialize all button actions like play
        // since it is not thread safe
        let queue = OperationQueue()
        queue.maxConcurrentOperationCount = 1 // serial queue
        
        //Save stuff to user data on exit
        NotificationCenter.default.addObserver(
            self,
            selector: #selector(saveData),
            name: UIApplication.willResignActiveNotification,
            object: nil
        )
        
        if let savedArray = UserDefaults.standard.array(forKey: "state") as? [Int64] {
            // Expecting 11 word state
            if savedArray.count == 8 {
                coins = []
                for i in 0..<8 {
                    state[i] = Int(savedArray[i])
                }
                trajectory = findtraj(Int32(state[4]), Int32(state[5]))-1
                print("state", savedArray)
            } else {
                print("stale user state")
            }
        } else {
            print("No state found.")
        }
        
        if let savedCoins = UserDefaults.standard.array(forKey: "coins") as? [Int] {
            coins = savedCoins
            print("coins", coins)
        }
        
        // If we didn't set the state from before, we will set up the coin array
        // with a randomized set of coin numbers
        if coins.count == 0 {
            for i in 0..<8 {
                state[i] = 0
            }
            while coins.count < ncoins() {
                coins.append(coins.count);
            }
            coins.shuffle()
        }
        print(coins)
        
        // Set an initial coin if not in state
        if state[4] == 0 && state[5] == 0 {
            print("Add a coin")
            newCoin()
        }
        

        leftButton.setImage(UIImage(named: "left_button"), for: .normal)
        rightButton.setImage(UIImage(named: "right_button"), for: .normal)
        spinButton.setImage(UIImage(named: "spin_button"), for: .normal)
        fireButton.setImage(UIImage(named: "fire_button"), for: .normal)
        
        turnLabel.text = "0"
        scoreLabel.text = String(coins.count+1)

        let leftAction = UIAction { _ in
            if self.gameOver { return }
            
            // We need to serialize the play and render since the C code is not thread safe
            queue.addOperation {
                self.play(command: "l")
            }
        }
        leftButton.addAction(leftAction, for: .touchUpInside)
        
        
        let rightAction = UIAction { _ in
            if self.gameOver { return }

            // We need to serialize the play and render since the C code is not thread safe
            queue.addOperation {
                self.play(command: "r")
            }
        }
        rightButton.addAction(rightAction, for: .touchUpInside)
        
        
        let spinAction = UIAction { _ in
            if self.gameOver { return }

            // We need to serialize the play and render since the C code is not thread safe
            queue.addOperation {
                self.play(command: "s")
            }
        }
        spinButton.addAction(spinAction, for: .touchUpInside)
        
        let fireAction = UIAction { _ in
            if self.gameOver { return }

            // We need to serialize the play and render since the C code is not thread safe
            queue.addOperation {
                self.play(command: "f")
            }
        }
        fireButton.addAction(fireAction, for: .touchUpInside)
        
        
        let hintAction = UIAction { _ in
            if self.gameOver { return }

            let targetX = self.trajectory % 18
            let targetSpin = self.trajectory / 18
            let arenaX = targetX * 32
            // Maybe we already have a hint and want to be lazy
            if self.hintButton.title(for: .normal) == "WINNER" {
                // you won
            } else if self.hintButton.title(for: .normal) != "Hint" {
                print("CHEAT", arenaX, targetSpin)
                self.state[2] = Int(arenaX)
                self.state[3] = Int(targetSpin)
                
                DispatchQueue.main.async {
                    self.hintButton.setTitle("Hint", for: .normal)
                }
                queue.addOperation {
                    self.play(command: "X")  // Just renders turn 0, X is not a command
                }
            } else {
                // We need to serialize the play and render since the C code is not thread safe
                print("HINT for trajectory", self.trajectory)
                var hint = "shoot"
                let currentX = self.state[2] / 32
                print("AT", currentX, "Need", targetX)
                if currentX < targetX {
                    hint = "right"
                } else if currentX > targetX {
                    hint = "left"
                } else if  targetSpin != self.state[3] {
                    hint = "spin"
                }
                DispatchQueue.main.async {
                    self.hintButton.setTitle(hint, for: .normal)
                }
            }
        }
        hintButton.addAction(hintAction, for: .touchUpInside)
        
        
        print("start")
        queue.addOperation {
            print("initial play state")
            /*
             state->turn          = state_array[0];
             state->points        = state_array[1];
             state->tri_x         = state_array[2];
             state->rotation      = state_array[3];
             state->coin_x        = state_array[4];
             state->coin_y        = state_array[5];
             state->cannon_t      = state_array[6];
             state->cannon_offset = state_array[7];
             */
            for i in 0..<8 {
                print(self.state[i])
            }
            self.play(command: "X")  // Just renders turn 0, X is not a command
        }


    }



    @objc func saveData() {
        // Make an array with our state and the set of coins left to shoot
        var array : [Int64] = Array(repeating: 0, count: 8)
        
        // Play state
        for i in 0..<8 {
            array[i] = Int64(state[i])
        }

        // The first 8 words are the Trifire state
        print("state",array)
        UserDefaults.standard.set(array, forKey: "state")
        print("coins", coins)
        UserDefaults.standard.set(coins, forKey: "coins")
    }

}

